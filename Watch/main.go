// Like code.google.com/p/rsc/cmd/Watch,
// but doesn't use kevent which doesn't seem
// to exist on Linux.
//
// A bunch of main is copied from the original.
package main

import (
	"log"
	"os/exec"
	"strings"
	"os"
	"time"
	"flag"
	"code.google.com/p/goplan9/plan9/acme"
)

func main() {
	flag.Parse()
	args := flag.Args()

	win, err := acme.New()
	if err != nil {
		log.Fatal("acme error: ", err)
	}

	pwd, _ := os.Getwd()
	win.Name(pwd + "/+watch")
	win.Ctl("clean")
	win.Fprintf("tag", "Get ")

	run := make(chan chan<- bool)
	go checker(run)
	go events(win, run)

	for {
		done := <-run
		cmd := exec.Command(args[0], args[1:]...)
		r, w, err := os.Pipe()
		if err != nil {
			log.Fatal(err)
		}
		win.Addr(",")
		win.Write("data", nil)
		win.Ctl("clean")
		win.Fprintf("body", "$ %s\n", strings.Join(args, " "))

		cmd.Stdout = w
		cmd.Stderr = w
		if err := cmd.Start(); err != nil {
			r.Close()
			w.Close()
			win.Fprintf("body", "%s: %s\n", strings.Join(args, " "), err)
			done <- true
			continue
		}

		w.Close()
		buf := make([]byte, 4096)
		for {
			n, err := r.Read(buf)
			if err != nil {
				break
			}
			win.Write("body", buf[:n])
		}
		if err := cmd.Wait(); err != nil {
			win.Fprintf("body", "%s: %s\n", strings.Join(args, " "), err)
		}
		win.Fprintf("body", "$\n")
		win.Fprintf("addr", "#0")
		win.Ctl("dot=addr")
		win.Ctl("show")
		win.Ctl("clean")
		done <- true
	}
}

func events(win *acme.Win, run chan chan<- bool) {
	done := make(chan bool)
	for e := range win.EventChan() {
		switch e.C2 {
		case 'x', 'X': // execute
			if string(e.Text) == "Get" {
				run <- done
				<- done
				continue
			}
			if string(e.Text) == "Del" {
				win.Ctl("delete")
			}
		}
		win.WriteEvent(e)
	}
	os.Exit(0)
}

func checker(run chan chan<- bool) {
	pwd, err := os.Getwd()
	if err != nil {
		log.Fatal(err)
	}

	last := time.Now()
	done := make(chan bool)
	known := make(map[string]bool)

	for {
		changed := false
		cur := make(map[string]bool)

		for _, n := range ents(pwd) {
			info, err := os.Stat(n)
			if err != nil && os.IsNotExist(err) {
				changed = true
				continue			
			}
			if err != nil {
				log.Fatal(err)
			}
			if !known[n] || last.Before(info.ModTime()) {
				changed = true
			}
			known[n] = true
			cur[n] = true
		}
	
		for n := range known {
			if !cur[n] {
				changed = true
				delete(known, n)
			}
		}

		if changed {
			run <- done
			<- done
			last = time.Now()
		}

		<- time.After(1)
	}
}

func ents(path string) []string {
	dir, err := os.Open(path)
	if err != nil {
		log.Fatal(err)
	}
	defer dir.Close()

	names, err := dir.Readdirnames(-1)
	if err != nil {
		log.Fatal(err)
	}

	return names
}