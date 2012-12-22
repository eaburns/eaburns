package kdtree

import (
	"math/rand"
	"reflect"
	"testing"
	"testing/quick"
)

// TestInsert tests the insert function, ensuring that random points
// inserted into an empty tree maintain the K-D tree invariant.
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

// TestMake tests the Make function, ensuring that a tree built
// using random points respects the K-D tree invariant.
func TestMake(t *testing.T) {
	if err := quick.Check(func(pts pointSlice) bool {
		nodes := make([]*Node, len(pts))
		for i, pt := range pts {
			nodes[i] = &Node{Point: pt}
		}
		tree := Make(nodes)
		_, ok := tree.node.invariantHolds()
		return ok
	}, nil); err != nil {
		t.Error(err)
	}
}

// TestInRange tests the InRange function, ensuring that all points
// in the range are reported, and all points reported are indeed in
// the range.
func TestInRange(t *testing.T) {
	if err := quick.Check(func(pts pointSlice, pt Point, r float64) bool {
		nodes := make([]*Node, len(pts))
		for i, pt := range pts {
			nodes[i] = &Node{Point: pt}
		}

		tree := Make(nodes)
		in := make(map[*Node]bool, len(nodes))
		for _, n := range tree.InRange(pt, r) {
			in[n] = true
		}

		num := 0
		for _, n := range nodes {
			if pt.sqDist(&n.Point) <= r*r {
				num++
				if !in[n] {
					return false
				}
			}
		}
		return num == len(in)
	}, nil); err != nil {
		t.Error(err)
	}
}

// TestPartition tests the partition function, ensuring that random
// points correctly partition into sets less than the pivot and a
// greater than or equal to the pivot.
func TestPartition(t *testing.T) {
	if err := quick.Check(func(pts pointSlice) bool {
		nodes := make([]*Node, len(pts))
		for i, Point := range pts {
			nodes[i] = &Node{Point: Point}
		}
		split := 0
		pivot := nodes[0].Point[split]
		nodes = nodes[1:]

		fst, snd := partition(split, pivot, nodes)
		for _, n := range fst {
			if n.Point[split] >= pivot {
				return false
			}
		}
		for _, n := range snd {
			if n.Point[split] < pivot {
				return false
			}
		}
		return true
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

// Generate implements the Generator interface for Points
func (p Point) Generate(r *rand.Rand, _ int) reflect.Value {
	for i := range p {
		p[i] = r.Float64()
	}
	return reflect.ValueOf(p)
}

// InvariantHolds returns the points in this subtree, and a bool
// that is true if the K-D tree invariant holds.  The K-D tree invariant
// states that all points in the left subtree have values less than that
// of the current node on the splitting dimension, and the points
// in the right subtree have values greater than or equal to that of
// the current node.
func (t *Node) invariantHolds() ([]Point, bool) {
	if t == nil {
		return []Point{}, true
	}

	left, leftOk := t.left.invariantHolds()
	right, rightOk := t.right.invariantHolds()

	ok := leftOk && rightOk

	if ok {
		for _, l := range left {
			if l[t.split] >= t.Point[t.split] {
				ok = false
				break
			}
		}
	}
	if ok {
		for _, r := range right {
			if r[t.split] < t.Point[t.split] {
				ok = false
				break
			}
		}
	}
	return append(append(left, t.Point), right...), ok
}
