// Cal is a time, date, and calendar program for acme(1)
package main

import (
	"code.google.com/p/goplan9/plan9/acme"
	"time"
	"io"
	"os"
	"os/exec"
	"strings"
)

func main() {
	win, err := acme.New()
	if err != nil {
		panic(err)
	}

	win.Name("/+Cal")
	showCal(win)

	fmt := " Font Mon Jan 2 15:04 2006"
	if len(os.Args) > 1 {
		fmt = " Font " + strings.Join(os.Args[1:], " ")
	}

	last := time.Now()
	for {
		now := time.Now()
		win.Ctl("cleartag")
		win.Write("tag", []byte(now.Format(fmt)))
		last = now

		if now.Month() != last.Month() {
			clear(win)
			showCal(win)
		}

		time.Sleep(time.Minute)
	}
}

type dataWriter struct {
	*acme.Win
}

func (d dataWriter) Write(data []byte) (int, error) {
	const maxWrite = 512
	total := 0
	for len(data) > 0 {
		sz := len(data)
		if sz > maxWrite {
			sz = maxWrite
		}
		n, err := d.Win.Write("data", data[:sz])
		if err != nil {
			return n, err
		}
		total += n
		data = data[n:]
	}
	return total, nil
}

func clear(win *acme.Win) {
	win.Addr("0,$")
	if _, err := win.Write("data", []byte{}); err != nil {
		panic(err)
	}
}

func showCal(win *acme.Win) {
	cmd := exec.Command("9", "cal")
	out, err := cmd.StdoutPipe()
	if err != nil {
		panic(err)
	}
	go cmd.Start()
	if _, err := io.Copy(dataWriter{win}, out); err != nil {
		panic(err)
	}
	if err := cmd.Wait(); err != nil {
		panic(err)
	}
	win.Ctl("clean")
}
