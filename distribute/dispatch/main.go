package main

import (
	"log"
	"os"
	"io"
	"bufio"
	"flag"
)

var (
	logfile = log.New(os.Stderr, "", log.LstdFlags)
	inpath = flag.String("cmdfile", "cmds", "The command file")
)

func main() {
	flag.Parse()
	finished := make(chan bool)
	joblist := newJoblist(finished)

	startWorkers(joblist)

	postCommands(joblist)

	<-finished
	logfile.Printf("%d jobs succeeded\n", joblist.nok)
	logfile.Printf("%d jobs failed\n", joblist.nfail)
	logfile.Printf("%d jobs completed\n", joblist.nok + joblist.nfail)
}

// postCommands reads the command file and posts
// each line as a command to the joblist.
func postCommands(joblist *joblist) {
	infile, err := os.Open(*inpath)
	if err != nil {
		logfile.Fatalf("failed to open %s: %s\n", *inpath, err)
	}

	in := bufio.NewReader(infile)
	for {
		switch str, prefix, err := in.ReadLine(); {
		case err != nil && err != io.EOF:
			logfile.Fatalf("failed to read line from %s: %s\n", *inpath, err)
		case err == io.EOF:
			joblist.eof <- true
			return
		case prefix:
			logfile.Fatalf("line is too long")
		default:
			joblist.postJob(string(str))
		}
	}
	panic("Unreachable")
}
