// A library for representing disjoint sets.
package djsets

// Set is a single disjoint set.  Its zero
// value is ready for use.
type Set struct{
	parent *Set
	rank int

	// Aux is a field that is free for
	// use by the user of this library.
	Aux interface{}
}

// Find returns a pointer the canonical Set
// that this Set is contained in.
func (s *Set) Find() *Set{
	if s.parent == s || s.parent == nil {
		s.parent = s
		return s
	}

	s.parent = s.parent.Find()
	return s.parent
}

// Union joins the two sets into the same
// canonical set.
func (a *Set) Union(b *Set) {
	switch ap, bp := a.Find(), b.Find(); {
	case ap == bp:
		return
	case ap.rank < bp.rank:
		ap.parent = bp
	default:
		bp.parent = ap
		if ap.rank == bp.rank {
			a.rank++
		}	
	}
}
