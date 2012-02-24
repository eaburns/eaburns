package strtree

import (
	"testing"
)

// TestCommonPrefixFunc tests the commonPrefix function
func TestCommonPrefixFunc(t *testing.T) {
	tests := [...]struct{ a, b, common string }{
		{"a", "b", ""},
		{"aa", "b", ""},
		{"aa", "ab", "a"},
		{"aa", "aa", "aa"},
		{"aa", "aab", "aa"},
	}

	for _, test := range tests {
		common := commonPrefix(test.a, test.b)
		if common != test.common {
			t.Errorf("a=[%s], b=[%s]: expected [%s], got [%s]\n",
				test.a, test.b, test.common, common)
		}
	}
}

// TestEmptyInsert1 tests inserting single elements into
// an empty tree.
func TestEmptyInsert1(t *testing.T) {
	tests := [...]string{
		"a",
		"ab",
		"abc",
	}

	for _, test := range tests {
		var tree T
		tree.Insert(test)
		if tree.prefix != test {
			t.Errorf("failed to insert %s: bad prefix", test)
		}
		if !tree.mem {
			t.Errorf("failed to insert %s: not memuded", test)
		}
	}
}

// TestInsertSame tests inserting the same element into
// a tree multiple times.
func TestInsertSame(t *testing.T) {
	tests := [...]string{
		"abc",
		"a",
		"",
	}

	for _, test := range tests {
		var tree T
		tree.Insert(test)
		tree.Insert(test)
		if tree.prefix != test {
			t.Fatalf("prefix changed, expected %s, got %s", test, tree.prefix)
		}
		if !tree.mem {
			t.Errorf("Element is not memuded")
		}
		if len(tree.kids) != 0 {
			t.Errorf("A child element node added")
		}
	}
}

// TestInsertSamePrefix inserts two strings, the 2nd has
// the same prefix as the 1st.  This function ensures
// that the resulting tree is formatted correctly.
func TestInsertSamePrefix(t *testing.T) {
	tests := [...]struct{ prefix, suffix string }{
		{"a", "bcd"},
		{"abc", "d"},
		{"abc", "def"},
		{"", "abc"},
	}

	for _, test := range tests {
		var tree T
		tree.Insert(test.prefix)
		tree.Insert(test.prefix + test.suffix)
		if tree.prefix != test.prefix {
			t.Errorf("prefix changed to %s", tree.prefix)
		}
		if !tree.mem {
			t.Errorf("prefix is no longer memuded")
		}
		if len(tree.kids) != 1 {
			t.Fatalf("expected 1 kid, got %d", len(tree.kids))
		}
		if !tree.kids[0].mem {
			t.Errorf("child isn't memuded")
		}
		if tree.kids[0].prefix != test.suffix {
			t.Errorf("expected child prefix to be %s, got %s",
				test.suffix, tree.kids[0].prefix)
		}
	}
}

// TestCommonPrefix tests inserting two strings with a common
// prefix, however, a split will be required.
func TestCommonPrefix(t *testing.T) {
	tests := [...]struct{ prefix, suffix0, suffix1 string }{
		{"abc", "d", "e"},
		{"", "a", "b"},
		{"abc", "d", ""},
	}

	for _, test := range tests {
		var tree T
		tree.Insert(test.prefix + test.suffix0)
		tree.Insert(test.prefix + test.suffix1)

		if tree.prefix != test.prefix {
			t.Errorf("expected prefix %s, got %s", test.prefix, tree.prefix)
		}

		if tree.mem && len(test.suffix0) > 0 && len(test.suffix1) > 0 {
			t.Errorf("prefix is incorrectly memuded")
		}

		nkids := 2
		if len(test.suffix0) == 0 || len(test.suffix1) == 0 {
			nkids = 1
		}

		if len(tree.kids) != nkids {
			t.Fatalf("expected %d kid, got %d", nkids, len(tree.kids))
		}

		for i := range tree.kids {
			ok := false
			if len(test.suffix0) > 0 && tree.kids[i].prefix == test.suffix0 {
				ok = true
			}
			if len(test.suffix1) > 0 && tree.kids[i].prefix == test.suffix1 {
				ok = true
			}
			if !ok {
				t.Errorf("unexpected child prefix expected either [%s] or [%s], got %s",
					test.suffix0, test.suffix1, tree.kids[i].prefix)
			}
		}
	}
}

// TestInsertRecur tests the recursive nature of insertion
func TestInsertRecur(t *testing.T) {
	var tree T
	tree.Insert("abc")
	tree.Insert("abcde")
	tree.Insert("abce")

	if len(tree.kids) != 2 {
		t.Fatalf("incorrect number of kids in initialization: %d", len(tree.kids))
	}
	dekid, ekid := -1, -1
	for i := range tree.kids {
		if tree.kids[i].prefix == "de" {
			dekid = i
		}
		if tree.kids[i].prefix == "e" {
			ekid = i
		}
	}
	if dekid < 0 {
		t.Fatalf("no child for de in initialization")
	}
	if ekid < 0 {
		t.Fatalf("no child for e in initialization")
	}

	tree.Insert("abcdef")
	if len(tree.kids[dekid].kids) != 1 {
		t.Errorf("expected 1 kid under de, got %d", len(tree.kids[dekid].kids))
	}
	if tree.kids[dekid].kids[0].prefix != "f" {
		t.Errorf("expected kid under de to have prefix f, got %s",
			tree.kids[dekid].kids[0].prefix)
	}
}

// TestMemberTrue inserts some strings and ensures that they
// are all marked as members.
func TestMemberTrue(t *testing.T) {
	strings := [...]string{
		"romane",
		"romanus",
		"romulus",
		"rubens",
		"ruber",
		"rubicon",
		"rubicundus",
	}
	var tree T
	for _, s := range strings {
		tree.Insert(s)
	}

	for _, s := range strings {
		if !tree.Member(s) {
			t.Errorf("%s is not a member", s)
		}
	}
}

// TestMemberFalse inserts some strings and ensures a set of
// strings, known not to be in the tree, are not members
func TestMemberFalse(t *testing.T) {
	strings := [...]string{
		"romane",
		"romanus",
		"romulus",
		"rubens",
		"ruber",
		"rubicon",
		"rubicundus",
	}
	var tree T
	for _, s := range strings {
		tree.Insert(s)
	}

	nonmems := [...]string{
		"r",
		"ro",
		"rom",
		"roma",
		"ru",
		"rubico",
		"abc",
		"",
	}
	for _, s := range nonmems {
		if tree.Member(s) {
			t.Errorf("%s is a member", s)
		}
	}
}

func TestIterate(t *testing.T) {
	// These strings are not sorted, "" is not in
	// the list of strings either.
	strings := [...]string{
		"rubens",
		"rubicon",
		"romulus",
		"rubicundus",
		"romane",
		"ruber",
		"romanus",
	}
	var tree T
	for _, s := range strings {
		tree.Insert(s)
	}

	n := 0
	last := ""	// not in the list so less than all.
	tree.Iterate(func(s string) {
		if last >= s {
			t.Fatalf("[%s] came before [%s]\n", last, s)
		}
		n++
		last = s
	})

	if n != len(strings) {
		t.Errorf("Only visited %d strings, expected %d\n", n, len(strings))
	}
}

func TestLen(t *testing.T) {
	tests := [...][]string{
		{},
		{ "a", "b", "c"},
		{
			"rubens",
			"rubicon",
			"romulus",
			"rubicundus",
			"romane",
			"ruber",
			"romanus",
		},
	}

	for _, test := range tests {
		var tree T
		for _, s := range test {
			tree.Insert(s)
		}
		l := tree.Len()
		if l != len(test) {
			t.Errorf("Expected a length of %d, got %d", len(test), l)
		}
	}
}