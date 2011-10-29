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