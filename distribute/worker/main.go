package main

import (
	"net"
	"net/http"
	"net/rpc"
	"os/exec"
	"errors"
	"strconv"
	"log"
	"flag"
)

var (
	connect = flag.String("c", "", "Address of the manager to connect to")
	port = flag.Int("p", 1234, "The port on which to listen")
)

type Worker struct{}

func (Worker) Execute(cmd *string, _ *struct{}) error {
	log.Printf("executing [%s]\n", *cmd)

	c := exec.Command("/bin/sh", "-c", *cmd)
	o, err := c.CombinedOutput()
	if err != nil {
		log.Printf("failed output=[%s]: %s\n", o, err)
		return errors.New(err.Error() + ": [" + string(o) + "]")
	}

	log.Print("succeeded")
	return nil
}

func main() {
	flag.Parse()

	rpc.Register(new(Worker))
	rpc.HandleHTTP()
	l, e := net.Listen("tcp", ":" + strconv.Itoa(*port))
	if e != nil {
		log.Fatal("listen error:", e)
	}
	log.Print("serving HTTP on ", l.Addr())

	if *connect == "" {
		log.Print("listening for connections")
		http.Serve(l, nil)
		return	// unreachable
	}

	log.Print("listening for connections")
	go http.Serve(l, nil)
	log.Printf("calling %s\n", *connect)
	client, err := rpc.DialHTTP("tcp", *connect)
	if err != nil {
		log.Fatalf("failed to connect to %s: %s", *connect, err)
	}
	var res struct{}
	client.Call("WorkerList.Add", l.Addr().String(), &res)
	client.Close()
	<-make(chan bool)	// go to sleep forever
}
