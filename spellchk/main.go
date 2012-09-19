// A simple spell checker that outputs acme
// addresses for each possibly mis-spelled
// word.
package main

import (
	"bufio"
	"flag"
	"fmt"
	"io"
	"io/ioutil"
	"log"
	"os"
	"strings"
	"unicode"
	"unicode/utf8"
)

// This file is available on Ubuntu.
const defaultDict = "/usr/share/dict/american-english"

var dict = flag.String("d", defaultDict, "specify the dictionary file")

type word struct {
	line, start int
	text        string
}

func main() {
	flag.Parse()

	dict := loadDict(*dict)
	path := flag.Arg(0)
	for w := range words(path) {
		// Ignore TeX stuff.
		if w.text[0] == '\\' {
			continue
		}
		word := strings.TrimRight(w.text, "\\")

		if correct(dict, word) {
			continue
		}
		fmt.Printf("%s:%d-+#%d	[%s]\n", path, w.line, w.start, w.text)
	}
}

func correct(dict map[string]bool, word string) bool {
	parts := strings.Split(word, "-")
	badPart := false
	for _, p := range parts {
		if p == "" {
			continue
		}
		if !dict[p] && !dict[strings.ToLower(p)] {
			badPart = true
		}
	}
	return !badPart
}

type Lexer struct {
	text   string
	start  int
	pos    int
	width  int
	runeno int
	lineno int
	bol    int
}

func words(path string) <-chan word {
	ws := make(chan word)
	go func(path string, ws chan<- word) {
		f, err := os.Open(path)
		if err != nil {
			log.Fatal(err)
		}
		text, err := ioutil.ReadAll(bufio.NewReader(f))
		if err != nil {
			log.Fatal(err)
		}
		f.Close()

		lex := &Lexer{text: string(text), lineno: 1}
		for {
			w, eof := lex.nextWord()
			ws <- w
			if eof {
				close(ws)
				return
			}
		}
	}(path, ws)
	return ws
}

const eof = -1

func (l *Lexer) next() rune {
	if l.pos >= len(l.text) {
		return eof
	}
	var r rune
	r, l.width = utf8.DecodeRuneInString(l.text[l.pos:])
	l.pos += l.width
	if r == '\n' {
		l.lineno++
		l.bol = l.pos
	}
	return r
}

func (l *Lexer) nextWord() (word, bool) {
	for {
		bol := l.bol
		lineno := l.lineno
		r := l.next()
		if r == eof || !wordRune(r) {
			w := word{
				line:  lineno,
				start: l.start - bol,
				text:  l.text[l.start : l.pos-l.width],
			}
			for {
				r = l.next()
				if r == eof || wordRune(r) {
					l.pos -= l.width
					break
				}
			}
			l.start = l.pos
			return w, r == eof
		}

	}
	panic("Unreachale")
}

func wordRune(r rune) bool {
	// Allow '\' so that we can choose to ignore TeX stuff.
	return unicode.IsLetter(r) || r == '\'' || r == '-' || r == '\\'
}

func loadDict(path string) map[string]bool {
	f, err := os.Open(path)
	if err != nil {
		log.Fatal(err)
	}
	defer f.Close()

	in := bufio.NewReader(f)
	dict := make(map[string]bool, 50000)
	for {
		line, err := in.ReadString('\n')
		if err == io.EOF {
			break
		}
		if err != nil {
			log.Fatal(err)
		}
		line = strings.TrimRight(line, "\r\n")
		dict[line] = true
	}
	return dict
}
