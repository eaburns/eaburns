// More portable implementation of
// code.google.com/p/rsc/cmd/Watch.
package main

import (
	"code.google.com/p/goplan9/plan9/acme"
	"github.com/howeyc/fsnotify"
	"io"
	"log"
	"os"
	"os/exec"
	"strings"
	"time"
)

type runRequest struct {
	time time.Time
	done chan<- bool
}

func main() {
	win, err := acme.New()
	if err != nil {
		log.Fatal(err)
	}
	wd, err := os.Getwd()
	if err != nil {
		log.Fatal(err)
	}
	win.Name(wd + "/+watch")
	win.Ctl("clean")
	win.Fprintf("tag", "Get ")

	run := make(chan runRequest)
	go events(win, run)
	go runner(win, run)
	watcher(wd, run)
}

// Everything but the FSN_CREATE flag since create
// seems to imply a modify.	
const watchFlags = fsnotify.FSN_MODIFY | fsnotify.FSN_DELETE | fsnotify.FSN_RENAME

func watcher(wd string, run chan<- runRequest) {
	w, err := fsnotify.NewWatcher()
	if err != nil {
		log.Fatal(err)
	}
	if err := w.WatchFlags(wd, watchFlags); err != nil {
		log.Fatal(err)
	}

	done := make(chan bool)
	for {
		select {
		case ev := <-w.Event:
			info, err := os.Stat(ev.Name)
			if os.IsNotExist(err) {
				info, err = os.Stat(wd)
			}
			if err != nil {
				log.Fatal(err)
			}
			run <- runRequest{ info.ModTime(), done }
			<-done

		case err := <-w.Error:
			log.Fatal(err)
		}
	}
}

func runner(win *acme.Win, reqs <-chan runRequest) {
	runCommand(win)
	last := time.Now()

	for req := range reqs {
		if last.Before(req.time) {
			runCommand(win)
			last = time.Now()
		}
		req.done <- true
	}
}

type BodyWriter struct {
	*acme.Win
}

func (b BodyWriter) Write(data []byte) (int, error) {
	return b.Win.Write("body", data)
}

func runCommand(win *acme.Win) {
	args := os.Args[1:]
	if len(args) == 0 {
		log.Fatal("Must supply a command")
	}
	cmdStr := strings.Join(args, " ")

	win.Addr(",")
	win.Write("data", nil)
	win.Ctl("clean")
	win.Fprintf("body", "$ %s\n", cmdStr)

	cmd := exec.Command(args[0], args[1:]...)
	r, w, err := os.Pipe()
	if err != nil {
		log.Fatal(err)
	}
	defer r.Close()
	cmd.Stdout = w
	cmd.Stderr = w

	if err := cmd.Start(); err != nil {
		win.Fprintf("body", "%s: %s\n", cmdStr, err)
		return
	}
	w.Close()
	io.Copy(BodyWriter{win}, r)
	if err := cmd.Wait(); err != nil {
		win.Fprintf("body", "%s: %s\n", cmdStr, err)
	}

	win.Fprintf("body", "%s\n", time.Now())
	win.Fprintf("addr", "#0")
	win.Ctl("dot=addr")
	win.Ctl("show")
	win.Ctl("clean")
}

func events(win *acme.Win, run chan<- runRequest) {
	done := make(chan bool)
	for e := range win.EventChan() {
		switch e.C2 {
		case 'x', 'X': // execute
			if string(e.Text) == "Get" {
				run <- runRequest{time.Now(), done}
				<-done
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
