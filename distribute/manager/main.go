package main

import (
	"log"
	"os"
	"strings"
	"net/rpc"
	"math/rand"
)

var (
	logfile = log.New(os.Stderr, "", log.LstdFlags)
	faillog = logger("fail.log")
	oklog = logger("ok.log")
)

type worker struct{
	addr string
	done chan bool
}

// newWorker creates a new worker at the given
// remote address.
func newWorker(addr string, jobs *joblist) *worker {
	w := &worker{
		addr: addr,
		done: make(chan bool),
	}
	go w.Go(jobs)
	return w
}

func (w *worker) Go(joblist *joblist) {
	logfile.Printf("worker %s: started\n", w.addr);

	client, err := rpc.DialHTTP("tcp", w.addr + ":1234")
	if err != nil {
		logfile.Printf("worker %s: %s", w.addr, err)
		w.done <- true
		return
	}
	logfile.Printf("worker %s: connected\n", w.addr)

	for j := range joblist.jobs {
		logfile.Printf("worker %s: got job [%s]\n", w.addr, j)

		var res struct{}
		err = client.Call("Worker.Execute", j, &res)
		if err != nil {
			if strings.HasPrefix(err.Error(), "exit status") {
				joblist.failJob(j, err.Error())
				continue
			}
			logfile.Printf("worker %s RPC error: %s\n", w.addr, err)
			joblist.repostJob(j)
			w.done <- true
			return
		}

		joblist.finishJob(j)
	}

	logfile.Printf("worker %s: done\n", w.addr)
	w.done <- true
}

const (
	resultOk = iota
	resultFail
	resultRepost
)

// result is the result of a job that a worker may pass back
// to the job list.
type result struct{
	status int
	cmd string
	output string
}

// joblist stores the different jobs and distributes them to
// workers upon request.
type joblist struct{
	q []string
	n, nok, nfail int
	goteof bool
	eof chan bool
	post chan string
	jobs chan string
	done chan result
}

// newJoblist makes a new joblist
func newJoblist() *joblist {
	j := &joblist{
		eof: make(chan bool),
		post: make(chan string),
		jobs: make(chan string),
		done: make(chan result),
	}
	go j.Go()
	return j
}

// postJob posts a new command to the joblist
func (j *joblist) postJob(cmd string) {
	j.post <- cmd
}

// failJob notifies the joblist that the given command failed
func (j *joblist) failJob(cmd string, output string) {
	j.done <- result{
		status: resultFail,
		cmd: cmd,
		output: string(output),
	}
}

// finishJob notifies the joblist that the given command
// completed successfully
func (j *joblist) finishJob(cmd string) {
	j.done <- result{
		status: resultOk,
		cmd: cmd,
	}
}

// repostJob notifies the joblist that the given command
// needs to be re-posted
func (j *joblist) repostJob(cmd string) {
	j.done <- result{
		status: resultRepost,
		cmd: cmd,
	}
}

func (j *joblist) Go() {
	for {
		for len(j.q) == 0 {
			select {
			case <- j.eof:
				logfile.Print("joblist: got EOF")
				j.goteof = true

			case p := <-j.post:
				logfile.Printf("joblist: got post [%s]\n", p)
				j.n++
				j.q = append(j.q, p)

			case p := <-j.done:
				if j.handleDone(p) { return }
			}
		}
		for len(j.q) > 0 {
			select {
			case <- j.eof:
				logfile.Print("joblist: got EOF")
				j.goteof = true

			case p := <-j.post:
				logfile.Printf("joblist: got post [%s]\n", p)
				j.n++
				j.q = append(j.q, p)

			case p := <-j.done:
				if j.handleDone(p) { return }

			case j.jobs <- j.q[len(j.q)-1]:
				logfile.Printf("joblist: sent [%s]\n", j.q[len(j.q)-1])
				j.q = j.q[:len(j.q)-1]
			}
		
		}
	}
}

// handleDone handles a result coming in on the done
// channel.  It returns true if all jobs are completed and
// there are no more jobs coming in from the command
// file.
func (j *joblist) handleDone(r result) bool {
	switch r.status {
	case resultOk:
		logfile.Printf("joblist: completed [%s]\n", r.cmd)
		oklog.Printf("[%s]\n", r.cmd)
		j.nok++
		j.n--

	case resultFail:
		logfile.Printf("joblist: failed [%s]\n", r.cmd)
		faillog.Printf("[%s] %s\n", r.cmd, r.output)
		j.nfail++
		j.n--

	case resultRepost:
		logfile.Printf("joblist: reposted [%s]\n", r.cmd)
		j.q = append(j.q, r.cmd)
	}

	if j.goteof && j.n == 0 {
		logfile.Print("joblist: all done")
		close(j.jobs)
		return true
	}

	return false
}

const (
	Nworkers = 2
	Njobs = 10
)

func main() {
	joblist := newJoblist()

	var workers []*worker
	w := newWorker("localhost", joblist)
	workers = append(workers, w)

	for i := 0; i < Njobs; i++ {
		if rand.Float32() < 0.5 {
			joblist.postJob("sleep 1")
		} else {
			joblist.postJob("echo This will fail; false")
		}
	}
	joblist.eof <- true

	for _, w := range workers {
		<-w.done
	}

	logfile.Printf("%d jobs succeeded\n", joblist.nok)
	logfile.Printf("%d jobs failed\n", joblist.nfail)
	logfile.Printf("%d jobs completed\n", joblist.nok + joblist.nfail)
}

// logger makes a new logger that logs to the given file
func logger(file string) *log.Logger {
	f, err := os.Create(file)
	if err != nil {
		logfile.Fatalf("failed to create log file %s: %s\n", file, err)
	}
	return log.New(f, "", log.LstdFlags)
}