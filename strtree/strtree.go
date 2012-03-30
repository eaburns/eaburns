// strtree implements a radix tree on strings
package strtree

import (
	"sort"
	"strings"
)

// A T is the root of a radix tree.  Each T represents a set
// of strings all with a common (possibly empty) prefix.
// The zero-value is ready to use.
type T struct {
	prefix string // The string prefix represented by this node
	kids   []T    // Trees containing extensions of this prefix
	mem    bool   // True if this string is in the set, otherwise false
}

// Insert inserts a string into the set
func (t *T) Insert(s string) {
	if s == t.prefix || (len(t.prefix) == 0 && t.mem == false) {
		t.prefix = s
		t.mem = true
		return
	}

	if strings.HasPrefix(s, t.prefix) {
		s = s[len(t.prefix):]
		i := search(t.kids, s)
		if i < 0 {
			t.kids = insert(t.kids, T{prefix: s, mem: true})
		} else {
			t.kids[i].Insert(s)
		}
		return
	}

	c := commonPrefix(t.prefix, s)
	suffix := T{prefix: t.prefix[len(c):], kids: t.kids, mem: t.mem}
	t.prefix = c
	t.kids = []T{suffix}
	if s == c {
		t.mem = true
		return
	}
	t.kids = insert(t.kids, T{prefix: s[len(c):], mem: true})
	t.mem = false
}

// commonPrefix returns the common prefix of two strings.
// This may be the empty string.
func commonPrefix(a, b string) string {
	var c []byte
	len := minLen(a, b)
	for i := 0; i < len; i++ {
		if a[i] != b[i] {
			break
		}
		c = append(c, a[i])
	}
	return string(c)
}

// minLen returns the length of the smaller of two strings.
func minLen(a, b string) int {
	if len(a) < len(b) {
		return len(a)
	}
	return len(b)
}

// Member returns true if the string is a member of the set
// and false otherwise.
func (t *T) Member(s string) bool {
	if s == t.prefix {
		return t.mem
	}

	if strings.HasPrefix(s, t.prefix) {
		s = s[len(t.prefix):]
		i := search(t.kids, s)
		if i < 0 {
			return false
		}
		return t.kids[i].Member(s)
	}

	return false
}

// Iterate calls a function on every string in the set in
// lexicographical order.
func (t *T) Iterate(f func(string)) {
	t.walk("", f)
}

// walk walks the tree in lexicographical order and calls
// a function on every string in the tree prefixed by the
// string given as a parameter.
func (t *T) walk(p string, f func(string)) {
	str := p + t.prefix
	if t.mem {
		f(str)
	}
	for i := range t.kids {
		t.kids[i].walk(str, f)
	}
}

// Len returns the number of strings in the set.  This operation
// is O(n) in the number of entries.
func (t *T) Len() int {
	n := 0
	if t.mem {
		n = 1
	}
	for i := range t.kids {
		n += t.kids[i].Len()
	}
	return n
}

// search returns the index for a string in the sorted
// slice or -1 if there is no index for that string yet.
func search(ts []T, s string) int {
	n := sort.Search(len(ts), func(i int) bool {
		return ts[i].prefix[0] >= s[0]
	})
	if n == len(ts) || ts[n].prefix[0] != s[0] {
		return -1
	}
	return n
}

// insert inserts a node into the slice in sorted order.
func insert(ts []T, t T) []T {
	ts = append(ts, t)
	i := len(ts) - 1
	for ; i > 0 && ts[i-1].prefix[0] > t.prefix[0]; i-- {
		ts[i] = ts[i-1]
	}
	ts[i] = t
	return ts
}
