package kdtree

import (
	"testing"
	"testing/quick"
	"math/rand"
	"reflect"
)

func TestInsert(t *testing.T) {
	if err := quick.Check(func(pts pointSlice) bool {
		var tree Root
		for _, p := range pts {
			tree.Insert(Point(p), nil)
		}
		_, ok := tree.node.invariantHolds()
		return ok
	}, nil); err != nil {
		t.Error(err)
	}
}

// A pointSlice is a slice of points that implements the quick.Generator
// interface, generating a random set of points on the unit square.
type pointSlice []Point

func (pointSlice) Generate(r *rand.Rand, size int) reflect.Value {
	ps := make([]Point, size)
	for i := range ps {
		for j := range ps[i] {
			ps[i][j] = r.Float64()
		}
	}
	return reflect.ValueOf(ps)
}

// InvariantHolds returns the points in this subtree, and a bool
// that is true if the K-D tree invariant holds.  The K-D tree invariant
// states that all points in the left subtree have values less than that
// of the current node on the splitting dimension, and the points
// in the right subtree have values greater than or equal to that of
// the current node.
func (t *node) invariantHolds() ([]Point, bool) {
	if t == nil {
		return []Point{}, true
	}

	left, leftOk := t.left.invariantHolds()
	right, rightOk := t.right.invariantHolds()

	ok := leftOk && rightOk

	if ok {	
		for _, l := range left {
			if l[t.split] >= t.pt[t.split] {
				ok = false
				break
			}
		}
	}
	if ok {
		for _, r := range right {
			if r[t.split] < t.pt[t.split] {
				ok = false
				break
			}
		}
	}
	return append(append(left, t.pt), right...), ok
}
