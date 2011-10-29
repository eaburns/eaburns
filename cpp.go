package cpp

import (
	"os"
	"fmt"
	"log"
	"bufio"
	"strings"
	"path/filepath"
)

type cpp struct{
	seen map[string]bool
	files []file
	buf []byte
}

type file struct{
	lineno int
	path string
	file *os.File
	in *bufio.Reader
}

func New(path string) (c *cpp, err os.Error) {
	c = &cpp{
		seen: make(map[string]bool),
	}
	err = c.push(path)
	return
}

func (cpp *cpp) Read(p []byte) (n int, err os.Error) {
	if cpp.buf != nil {
		return cpp.fillResult(p, cpp.buf), nil
	}

	if cpp.top() == nil {
		return 0, os.EOF
	}

	line, err := cpp.readLine()
	if err != nil && err == os.EOF {
		cpp.pop()
		if len(line) > 0 {
			return cpp.fillResult(p, []byte(line)), nil
		}
		return cpp.Read(p)
	} else if err != nil {
		return 0, err
	}

	line = strings.Trim(line, " \t")
	switch {
	case line[0] != '#':
		return cpp.fillResult(p, []byte(line)), nil
	case strings.HasPrefix(line, "#include"):
		return cpp.include(p, line)
	default:
		log.Printf("Got directive [%s]\n", line);
		return cpp.Read(p)
	}
	panic("Unreachable")
}

func (cpp *cpp) include(p []byte, line string) (int, os.Error) {
	inc, err := cpp.getInclude(line)
	if err != nil {
		return 0, err
	}

	if inc[0] != '/' {
		dir, _ := filepath.Split(cpp.top().path)
		inc = filepath.Join(dir, inc)
	}

	err = cpp.push(inc);
	if err != nil {
		return 0, err
	}
	return cpp.Read(p)
}

func (cpp *cpp) getInclude(line string) (string, os.Error) {
	start := strings.IndexAny(line, "\"<")
	if start < 0 {
		return "", cpp.errorf("no starting delimiter\n")
	}
	line = line[start:]

	endsep := '"'
	if line[0] == '<' {
		endsep = '>'
	}
	line = line[1:]

	end := strings.IndexRune(line, endsep)
	if end < 0 {
		return "", cpp.errorf("no ending delimiter\n")
	}
	return line[:end], nil
}

func (cpp *cpp) fillResult(p []byte, line []byte) int {
	n := copy(p, line)
	if n < len(line) {
		cpp.buf = line[n:len(line)]
	} else {
		cpp.buf = nil
	}
	return n
}

func (cpp *cpp) push(path string) os.Error {
	if _, ok := cpp.seen[path]; ok {
		loop := []string{}
		for i := range cpp.files {
			loop = append(loop, cpp.files[i].path)
		}
		return fmt.Errorf("Include loop: %v", append(loop, path))
	}
	in, err := os.Open(path)
	if err != nil {
		return err
	}
	f := file{
		lineno: 1,
		path: path,
		file: in,
		in: bufio.NewReader(in),
	}
	cpp.seen[path] = true
	cpp.files = append(cpp.files, f)
	return nil
}

func (cpp *cpp) pop() {
	cpp.seen[cpp.top().path] = false
	cpp.top().file.Close()
	if len(cpp.files) == 1 {
		cpp.files = []file{}
	} else {	
		cpp.files = cpp.files[:len(cpp.files)-1]
	}
}

func (cpp *cpp) top() *file {
	if len(cpp.files) == 0 {
		return nil
	}
	return &cpp.files[len(cpp.files)-1]
}

func (cpp *cpp) errorf(f string, args ...interface{}) os.Error {
	prefix := fmt.Sprintf("%s:%s: ", cpp.top().path, cpp.top().lineno)
	suffix := fmt.Sprintf(f, args)
	return fmt.Errorf("%s%s", prefix, suffix)
}

func (cpp *cpp) readLine() (string, os.Error) {
	buf := make([]byte, 0, 100)
	data, prefix, err := cpp.top().in.ReadLine()
	for err == nil && len(data) > 0 && (prefix || data[len(data)-1] == '\\') {
		if !prefix && data[len(data)-1] == '\\' {
			cpp.top().lineno++
			data = data[:len(data)-1]
		}
		buf = append(buf, data...)
		data, prefix, err = cpp.top().in.ReadLine()
	}
	if err == nil {
		buf = append(buf, data...)
	}
	cpp.top().lineno++
	return string(buf), err
}
