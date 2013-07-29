package djsets

import "testing"

// TestFindSelf tests the Find() method
// on a set that is its own parent.
func TestFindSelf(t *testing.T) {
	var s Set
	if s.Find() != &s {
		t.Error("Did not find itself")
	}
	if s.Find() != &s {
		t.Error("Did not find itself")
	}
}

// TestUnion tests that two sets that have
// been Union()ed indeed point to the
// same canonical set.
func TestUnion(t *testing.T) {
	var a, b Set
	a.Union(&b)

	if a.Find() != b.Find() {
		t.Error("Unioned sets point to different parents")
	}

	if a.Find() != &a && a.Find() != &b {
		t.Error("First set point to a strange parent")
	}
	if b.Find() != &a && b.Find() != &b {
		t.Error("Second set point to a strange parent")
	}
}
