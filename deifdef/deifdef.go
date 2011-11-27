package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

var defs = map[string]string{}

func main() {
	for i, a := range os.Args {
		if i > 0 {
			arg(a)
		}
	}
	cpp := mk(bufio.NewReader(os.Stdin), bufio.NewWriter(os.Stdout))
	cpp.preproc()
	cpp.out.Flush()
}

func arg(s string) {
	if len(s) > 2 && s[0] == '-' && s[1] == 'D' {
		defarg(s[2:len(s)])
	} else {
		fmt.Fprintf(os.Stderr, "Ignoring argument %s\n", s)
	}
}

func defarg(s string) {
	eq := strings.Index(s, "=")
	if eq < 0 {
		defs[s] = "0"
	} else {
		id := s[0:eq]
		vl := s[eq+1 : len(s)]
		defs[id] = vl
	}
}
