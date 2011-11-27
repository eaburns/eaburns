package main

import (
	"fmt"
	"strconv"
	"os"
	"bufio"
	"path/filepath"
)

func (c *cpp) proc(line []byte) (output bool) {
	lineno = c.lineno
	toks := scan(line)
	root := parse(toks)
	c.eval(root)
	kind := toks[0].t
	return !c.ignoring() && (kind == tokDef || kind == tokUndef || kind == tokInc)
}

type junker int

func (junker) Write(p []byte) (n int, err os.Error) {
	return len(p), nil
}

func (c *cpp) eval(n *nd) {
	switch n.t {
	case tokInc:
		if !c.ignoring() {
			c.evalInc(n)
		}
	case tokIf:
		var ok bool
		if !c.ignoring() {
			ok = c.evalRel(n.left)
		}
		c.pushif(ok)
	case tokIfdef:
		var ok bool
		if !c.ignoring() {
			_, ok = defs[n.sval]
		}
		c.pushif(ok)
	case tokIfndef:
		var ok bool = true
		if !c.ignoring() {
			_, ok = defs[n.sval]
		}
		c.pushif(!ok)
	case tokEndif:
		c.popif()
	case tokElse:
		vl := !c.ignoring()
		c.popif()
		c.pushif(!vl)
	case tokUndef:
		if c.ignoring() {
			return
		}
		defs[n.sval] = "", false
	case tokDef:
		if c.ignoring() {
			return
		}
		if n.left == nil {
			defs[n.sval] = "0"
			return
		}
		if n.left.t != tokStr && n.left.t != tokNum && n.left.t != tokId {
			panic(fmt.Sprintf("line %d: Invalid define value", c.lineno))
		}
		defs[n.sval] = n.left.sval
	default:
		panic(fmt.Sprintf("Invalid directive: %v", n))
	}
}

func (c *cpp) evalInc(n *nd) {
	s := c.evalStr(n.left)
	if s[0] == '<' {
		return		// ignore system includes
	}
	dir, fname := filepath.Split(s[1 : len(s) - 1])
	popdir := pushdir(dir)
	f, err := os.Open(fname)
	defer func () { f.Close() } ()
	if err != nil {
		panic(fmt.Sprintf("line %d: %s", c.lineno, err))
	}
	cpp := mk(bufio.NewReader(f), bufio.NewWriter(junker(0)))
	cpp.preproc()
	popdir()
}

// Changes to the new directory and returns a function that pops back
// to the previsou directory.
func pushdir(newdir string) (func ()) {
	curdir, err := os.Getwd()
	if err != nil {
		panic("Failed to get current directory")
	}
	if newdir != "" {
		os.Chdir(newdir)
	}
	return func () {
		if newdir == "" {
			return
		}
		err = os.Chdir(curdir)
		if err != nil {
			panic("Failed to reset directory")
		}
	}
}

func (c *cpp) evalStr(n *nd) string {
	switch n.t {
	case tokStr:
		return n.sval
	case tokId:
		return defs[n.sval]
	}
	panic(fmt.Sprintf("line %d: expected string", c.lineno))
}

func (c *cpp) evalRel(n *nd) bool {
	switch n.t {
	case tokNot:
		return !c.evalRel(n.left)
	case tokEq:
		return c.evalNum(n.left) == c.evalNum(n.right)
	case tokNe:
		return c.evalNum(n.left) != c.evalNum(n.right)
	case tokGe:
		return c.evalNum(n.left) >= c.evalNum(n.right)
	case tokLe:
		return c.evalNum(n.left) <= c.evalNum(n.right)
	case tokLt:
		return c.evalNum(n.left) < c.evalNum(n.right)
	case tokGt:
		return c.evalNum(n.left) > c.evalNum(n.right)
	case tokAnd:
		return c.evalRel(n.left) && c.evalRel(n.right)
	case tokOr:
		return c.evalRel(n.left) || c.evalRel(n.right)
	case tokDefed:
		_, ok := defs[n.sval]
		return ok
	case tokNum, tokId:
		return c.evalNum(n) != 0
	}
	panic(fmt.Sprintf("line %d: Invalid expression parse: %v", c.lineno, n))
}

func (c *cpp) evalNum(n *nd) int {
	switch n.t {
	case tokNum:
		return n.nval
	case tokId:
		vl, ok := defs[n.sval]
		if !ok {
			return 0
		}
		vl = string(num.Find([]byte(vl)))
		i, err := strconv.Atoi(vl)
		if err != nil {
			panic(fmt.Sprintf("line %d: Invalid number: %s", c.lineno, vl))
		}
		return i
	}
	panic(fmt.Sprintf("line %d: Invalid number: %v", c.lineno, n.t))
}

func (c *cpp) ignoring() bool {
	return !c.ifs[c.nifs-1]
}

func (c *cpp) pushif(truth bool) {
	if c.nifs == Stksz-1 {
		panic("Frame stack overflow")
	}
	c.ifs[c.nifs] = c.ifs[c.nifs-1] && truth
	c.nifs++
}

func (c *cpp) popif() {
	if c.nifs == 0 {
		panic("Frame stack underflow")
	}
	c.nifs--
}
