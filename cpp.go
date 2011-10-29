// Defines an io.Reader that runs its data
// through a C preprocessor before it is
// read by the user.

package cpp

import (
	"os"
	"fmt"
	"log"
	"bufio"
	"strings"
	"path/filepath"
)

const whiteSpace = " \t"

type Cpp struct{
	defs map[string]string
	nconds int	// number of conditions
	nfalse int		// depth inside a false cond
	onstack map[string]bool
	files []file
	buf []byte	// left over bytes that were not yet read
}

type file struct{
	lineno int
	path string
	file *os.File
	in *bufio.Reader
}

// Create a new pre-processor reading from the
// given file.
func New(path string) (c *Cpp, err os.Error) {
	c = &Cpp{
		defs: make(map[string]string),
		onstack: make(map[string]bool),
	}
	err = c.push(path)
	return
}

// Read the preprocessed data.  This method
// never returns more than a single line of the
// file at a time.
func (cpp *Cpp) Read(p []byte) (n int, err os.Error) {
	if cpp.buf != nil {
		return cpp.fillResult(p, cpp.buf), nil
	}

	if cpp.top() == nil {
		return 0, os.EOF
	}

	line, raw, err := cpp.readLine()
	if err != nil && err == os.EOF {
		cpp.pop()
		if len(line) > 0 {
			return cpp.fillResult(p, []byte(line)), nil
		}
		return cpp.Read(p)
	} else if err != nil {
		return 0, err
	}

	line = strings.Trim(line, whiteSpace)
	switch {
	case line[0] != '#':
		if cpp.nfalse == 0 {
			return cpp.fillResult(p, []byte(raw)), nil
		}
		return cpp.Read(p)

	case strings.HasPrefix(line, "#include"):
		return cpp.include(p, rmDirective(line))

	case strings.HasPrefix(line, "#define"):
		return cpp.define(p, rmDirective(line))

	case strings.HasPrefix(line, "#ifdef"):
		return cpp.ifDef(p, rmDirective(line))

	case strings.HasPrefix(line, "#ifndef"):
		return cpp.ifNDef(p, rmDirective(line))

	case strings.HasPrefix(line, "#endif"):
		return cpp.endIf(p, rmDirective(line))

	default:
		log.Printf("Got directive [%s]\n", line);
		return cpp.Read(p)
	}
	panic("Unreachable")
}

func (cpp *Cpp) include(p []byte, line string) (int, os.Error) {
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

// Get the name of the file which is being included
// by an include directive.
func (cpp *Cpp) getInclude(line string) (string, os.Error) {
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

func (cpp *Cpp) define(p []byte, line string) (int, os.Error) {
	id, vl := line, ""
	if i := strings.IndexAny(line, whiteSpace); i >= 0 {
		id, vl = line[:i], strings.Trim(line[i:], whiteSpace)
	}
	cpp.defs[id] = vl
	return cpp.Read(p)
}

func (cpp *Cpp) ifDef(p []byte, line string) (int, os.Error) {
	id := line
	if i := strings.IndexAny(line, whiteSpace); i >= 0 {
		id = line[:i]
	}
	_, ok := cpp.defs[id]
	cpp.cond(ok)
	return cpp.Read(p)
}

func (cpp *Cpp) ifNDef(p []byte, line string) (int, os.Error) {
	id := line
	if i := strings.IndexAny(line, whiteSpace); i >= 0 {
		id = line[:i]
	}
	_, ok := cpp.defs[id]
	cpp.cond(!ok)
	return cpp.Read(p)
}

func (cpp *Cpp) endIf(p []byte, line string) (int, os.Error) {
	if cpp.nconds == 0 {
		return 0, cpp.errorf("#endif without matching condition")
	}
	cpp.nconds--
	if cpp.nfalse > 0 {
		cpp.nfalse--
	}
	return cpp.Read(p)
}

// Track the given condition.
func (cpp *Cpp) cond(b bool) {
	cpp.nconds++
	if !b || cpp.nfalse > 0 {
		cpp.nfalse++
	}
}

// Removes the directive from the beginning
// of the line and trims off any leading/trailing
// whitespace.
func rmDirective(line string) string {
	line = strings.Trim(line, whiteSpace)
	if line[0] != '#' {
		panic("Not called with a directive")
	}
	i := strings.IndexAny(line, whiteSpace)
	if i < 0 {
		return ""
	}
	return strings.Trim(line[i:], whiteSpace)
}

// Copies as many bytes as possible into p
// from the line.  If p cannot hold the entire
// line then the rest of it is put in the line
// buffer.
func (cpp *Cpp) fillResult(p []byte, line []byte) int {
	n := copy(p, line)
	if n < len(line) {
		cpp.buf = line[n:len(line)]
	} else {
		cpp.buf = nil
	}
	return n
}

// Push the path onto the top of the file stack.
func (cpp *Cpp) push(path string) os.Error {
	if  cpp.onstack[path] {
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
	cpp.onstack[path] = true
	cpp.files = append(cpp.files, f)
	return nil
}

// Pop the top file off of the stack.
func (cpp *Cpp) pop() {
	cpp.onstack[cpp.top().path] = false
	cpp.top().file.Close()
	if len(cpp.files) == 1 {
		cpp.files = []file{}
	} else {	
		cpp.files = cpp.files[:len(cpp.files)-1]
	}
}

// Get the top file from the stack or nil if there is
// no current file.
func (cpp *Cpp) top() *file {
	if len(cpp.files) == 0 {
		return nil
	}
	return &cpp.files[len(cpp.files)-1]
}

// Format an error with the current file and line.
func (cpp *Cpp) errorf(f string, args ...interface{}) os.Error {
	prefix := fmt.Sprintf("%s:%d: ", cpp.top().path, cpp.top().lineno)
	suffix := f
	if len(args) > 0 {
		suffix = fmt.Sprintf(f, args)
	}
	return fmt.Errorf("%s%s", prefix, suffix)
}

// Read a line from the top file on the stack.
// Returns the full line (with escaped newlines
// removed), the raw line (with escaped newlines
// intact and any error that may have occured.
func (cpp *Cpp) readLine() (string, string, os.Error) {
	line := make([]byte, 0, 100)
	raw := make([]byte, 0, 100)

	data, prefix, err := cpp.top().in.ReadLine()
	for err == nil && len(data) > 0 && (prefix || data[len(data)-1] == '\\') {
		raw = append(raw, data...)
		if !prefix {
			cpp.top().lineno++
			raw = append(raw, '\n')
			if data[len(data)-1] == '\\' {
				data = data[:len(data)-1]
			}
		}
		line = append(line, data...)
		data, prefix, err = cpp.top().in.ReadLine()
	}

	if err == nil {
		raw = append(raw, append(data, '\n')...)
		if data[len(data)-1] == '\\' {
			data = data[:len(data)-1]
		}
		line = append(line, data...)
	}
	cpp.top().lineno++

	return string(line), string(raw), err
}
