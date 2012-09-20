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
	"flag"
)

var path = flag.String("p", ".", "specify the path to watch")

func main() {
	flag.Parse()

	win, err := acme.New()
	if err != nil {
		log.Fatal(err)
	}

	p := *path
	if p == "." {
		p, err = os.Getwd()
		if err != nil {
			log.Fatal(err)
		}
 	}

	win.Name(p + "/+watch")
	win.Ctl("clean")
	win.Fprintf("tag", "Get ")

	run := make(chan runRequest)
	go events(win, run)
	go runner(win, run)
	watcher(p, run)
}

// A runRequests is sent to the runner to request
// that the command be re-run.
type runRequest struct {
	// Time is the times for the request.  This
	// is either the modification time of a
	// changed file, or the time at which a
	// Get event was sent to acme.
	time time.Time

	// Done is a channel upon which the runner
	// should signal its completion.
	done chan<- bool
}

// Everything but the FSN_CREATE flag since create
// seems to imply a modify.	
const watchFlags = fsnotify.FSN_MODIFY | fsnotify.FSN_DELETE | fsnotify.FSN_RENAME

// Watcher watches the directory and sends a
// runRequest when the watched path changes.
func watcher(path string, run chan<- runRequest) {
	w, err := fsnotify.NewWatcher()
	if err != nil {
		log.Fatal(err)
	}
	if err := w.WatchFlags(path, watchFlags); err != nil {
		log.Fatal(err)
	}

	done := make(chan bool)
	for {
		select {
		case ev := <-w.Event:
			info, err := os.Stat(ev.Name)
			if os.IsNotExist(err) {
				info, err = os.Stat(path)
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

// Runner runs the commond upon
// receiving an up-to-date runRequest.
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

// BodyWriter implements io.Writer, writing
// to the body of an acme window.
type BodyWriter struct {
	*acme.Win
}

func (b BodyWriter) Write(data []byte) (int, error) {
	return b.Win.Write("body", data)
}

// RunCommand runs the command and sends
// the result to the given acme window.
func runCommand(win *acme.Win) {
	args := flag.Args()
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

// Events handles events coming from the
// acme window.
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
