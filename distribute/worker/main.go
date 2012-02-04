package main

import (
	"net"
	"net/http"
	"net/rpc"
	"os/exec"
	"errors"
	"log"
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
	rpc.Register(new(Worker))
	rpc.HandleHTTP()
	l, e := net.Listen("tcp", ":1234")
	if e != nil {
		log.Fatal("listen error:", e)
	}
	log.Print("serving HTTP on ", l.Addr())
	http.Serve(l, nil)
}
