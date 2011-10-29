// This is not really how gotest is suppose to
// be used.  Eventually this should be done
// correctly.

package cpp

import (
	"os"
	"fmt"
	"testing"
)

func TestIgnoreDirectives(t *testing.T) {
	cpp, err := New("testfiles/a")
	if err != nil {
		t.Fatal(err)
	}

	line := make([]byte, 100)
	n, err := cpp.Read(line)
	for err == nil {
		fmt.Printf("[%s]\n", string(line[:n]))
		n, err = cpp.Read(line)
	}

	if err != os.EOF {
		t.Fatal(err)
	}
}

func TestIfdef(t *testing.T) {
	cpp, err := New("testfiles/ifdef")
	if err != nil {
		t.Fatal(err)
	}

	line := make([]byte, 100)
	n, err := cpp.Read(line)
	for err == nil {
		fmt.Printf("[%s]\n", string(line[:n]))
		n, err = cpp.Read(line)
	}

	if err != os.EOF {
		t.Fatal(err)
	}
}