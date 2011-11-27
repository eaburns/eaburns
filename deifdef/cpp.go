package main

import (
	"bufio"
	"io"
	"fmt"
)

const (
	Stksz = 100
)

type cpp struct {
	lineno int
	in  *bufio.Reader
	out *bufio.Writer

	nifs int
	ifs [Stksz]bool
}

func mk(in *bufio.Reader, out *bufio.Writer) cpp {
	return cpp{
	in: in,
	out: out,
	nifs: 1,
	ifs: [Stksz]bool{0: true},
	};
}

func (c *cpp) preproc() {
	line, err := c.readl()
	for err == nil {
		//ln := lineno
		pr := c.seeline(line)
		if pr != nil {
			//out.Write([]byte(fmt.Sprintf("%4d: ", ln)))
			c.out.Write(pr)
			c.out.Write([]byte{'\n'})
		}
		line, err = c.readl()
	}
	//fmt.Fprintf(out, "%v\n", err)
}

// Handle an individual line
func (c *cpp) seeline(line []byte) []byte {
	if isdrctv(chompspace(line)) {
		return c.procdrctv(line)
	}
	if c.ignoring() {
		return nil
	}
	return line
}

// Process a directive
func (c *cpp) procdrctv(line []byte) []byte {
	line, raw := c.fullline(line)
	output := c.proc(line)
	if output {
		return raw
	}
	return nil
}

func (cpp *cpp) readl() (line []byte, err error) {
	line = []byte{}
	cpp.lineno++
	for {
		c, err := cpp.in.ReadByte()
		if err != nil {
			return nil, err
		}
		if c == '\n' {
			break
		}
		line = append(line, c)
	}
	r, err := cpp.in.Peek(1)
	if err != nil {
		return line, nil
	}
	if r[0] == '\r' {
		cpp.in.ReadByte()
		line = append(line, r[0])
	}
	return
}

// Handle escaped new lines
//
// Returns the logical line (newlines and escapes removed) and the raw
// bytes (nothing removed)
func (c cpp) fullline(line []byte) ([]byte, []byte) {
	raw := make([]byte, len(line))
	if copy(raw, line) != len(line) {
		panic("Failed to copy input line")
	}
	for line[len(line)-1] == '\\' {
		nxt, err := c.readl()
		if err != nil {
			if err == io.EOF {
				return line, raw
			}
			panic(fmt.Sprintf("line %d: unexpected EOF", c.lineno))
		}
		raw = append(raw, '\n')
		raw = append(raw, nxt...)
		line = append(line[0:len(line)-1], nxt...)
	}
	return line, raw
}

// Is this a directive that we need to handle?
func isdrctv(line []byte) bool {
	if !direct.Match(line) {
		return false
	}
	matches := direct.FindSubmatch(line)
	_, ok := directives[string(matches[1])]
	return ok
}

// Eat beginning of line white-space.
func chompspace(b []byte) []byte {
	for i := 0; i < len(b); i++ {
		if b[i] != ' ' && b[i] != '\t' {
			return b[i:len(b)]
		}
	}
	return []byte{}
}
