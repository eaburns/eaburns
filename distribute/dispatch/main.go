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
	for err == nil {
		var str string
		switch str, err = readLine(in); {
		case err != nil && err != io.EOF:
			logfile.Fatalf("failed to read line from %s: %s\n", *inpath, err)
		case err == nil:
			joblist.postJob(str)
		}
	}

	joblist.eof <- true
}

// readLine reads a single line from a file.  An error
// is returned if the read fails.
func readLine(in *bufio.Reader) (string, error) {
	buf := make([]byte, 0, 100)
	data, prefix, err := in.ReadLine()
	for err == nil && len(data) > 0 && (prefix || data[len(data)-1] == '\\') {
		if !prefix && data[len(data)-1] == '\\' {
			data = data[:len(data)-1]
		}
		buf = append(buf, data...)
		data, prefix, err = in.ReadLine()
	}
	if err == nil {
		buf = append(buf, data...)
	}
	return string(buf), err
}
