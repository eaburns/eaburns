// Kdtree is a very simple K-D tree implementation.
// This implementation uses a fixed value for K.  The intention
// is to copy the code locally, change K to your needs, and
// change Node.Data's type to suit your needs too.
package kdtree

import (
	"sort"
)

// K is the number of dimensions of the stored points.
const K = 2

// Point is a location in K-dimensional space.
type Point [K]float64

// SqDist returns the square distance between two points.
func (a *Point) sqDist(b *Point) float64 {
	sqDist := 0.0
	for i, x := range a {
		diff := x - b[i]
		sqDist += diff * diff
	}
	return sqDist
}

// Root is the root of a K-D tree.  The zero-value is an empty tree.
type Root struct {
	node *Node
}

// Make returns a new K-D tree built using the given nodes.
func Make(nodes []*Node) Root {
	if len(nodes) == 0 {
		return Root{}
	}
	return Root{buildTree(0, nodes)}
}

// Insert inserts data associated with a given point into the KD tree.
func (r *Root) Insert(pt Point, data interface{}) {
	r.node = r.node.insert(0, pt, data)
}

// InRange returns all of the nodes in the K-D tree with a point within
// a distance r from the given point.
func (r *Root) InRange(pt Point, radius float64) []*Node {
	if radius < 0 {
		return []*Node{}
	}
	return r.node.inRange(&pt, radius, nil)
}

// InRangeSlice is the same as InRange, however, a pre-allocated
// slice is used for the returned nodes.  Note that, if the pre-allocated
// slice is not large enough, then the returned slice will be a newly
// allocated slice that can fit all of the nodes.
func (r *Root) InRangeSlice(pt Point, radius float64, slice []*Node) []*Node {
	slice = slice[:0]
	if radius < 0 {
		return slice
	}
	return r.node.inRange(&pt, radius, slice)
}

// Height returns the height (the maximum length path to a leaf)
// of the K-D tree.
//
// This operation uses time linear in the number of nodes in the tree.
func (r *Root) Height() int {
	return r.node.height()
}

// A Node is a node in the K-D tree, pairing a point in K-dimensional
// space with a value.
type Node struct {
	// Point is the K-dimensional point associated with the
	// data of this node.
	Point
	// Data is auxiliary data associated with the point of this node.
	Data interface{}

	split       int
	left, right *Node
}

// Insert inserts the point, data pair beneath the given node, returning
// a new node rooting the new subtree.
func (t *Node) insert(depth int, pt Point, data interface{}) *Node {
	if t == nil {
		return &Node{Point: pt, split: depth % K, Data: data}
	}
	if pt[t.split] < t.Point[t.split] {
		t.left = t.left.insert(depth+1, pt, data)
	} else {
		t.right = t.right.insert(depth+1, pt, data)
	}
	return t
}

// InRange returns a slice of all of the nodes within the given
// squared distance of the point.
func (t *Node) inRange(pt *Point, r float64, nodes []*Node) []*Node {
	if t == nil {
		return nodes
	}

	diff := pt[t.split] - t.Point[t.split]

	thisSide, otherSide := t.right, t.left
	if diff < 0 {
		thisSide, otherSide = t.left, t.right
		diff = -diff // abs
	}
	nodes = thisSide.inRange(pt, r, nodes)
	if diff <= r {
		if t.Point.sqDist(pt) < r*r {
			nodes = append(nodes, t)
		}
		nodes = otherSide.inRange(pt, r, nodes)
	}

	return nodes
}

// Height returns the height of this node.
func (t *Node) height() int {
	if t == nil {
		return 0
	}
	ht := t.left.height()
	if rht := t.right.height(); rht > ht {
		ht = rht
	}
	return ht + 1
}

// BuildTree returns a new tree, built up from the given slice of nodes.
func buildTree(depth int, nodes []*Node) *Node {
	split := depth % K
	switch len(nodes) {
	case 0:
		return nil
	case 1:
		nd := nodes[0]
		nd.split = split
		nd.left, nd.right = nil, nil
		return nd
	}
	cur, nodes := med(split, nodes)
	left, right := partition(split, cur.Point[split], nodes)
	cur.split = split
	cur.left = buildTree(depth+1, left)
	cur.right = buildTree(depth+1, right)
	return cur
}

// Partition returns two node slices, the first containing all elements
// with values less than that of the pivot on the split dimension, and the
// second containing all values greater or equal to that of the pivot
// on the splitting dimension.
func partition(split int, pivot float64, nodes []*Node) (fst, snd []*Node) {
	p := 0
	for i, nd := range nodes {
		if nd.Point[split] < pivot {
			nodes[p], nodes[i] = nodes[i], nodes[p]
			p++
		}
	}
	return nodes[:p], nodes[p:]
}

// Med returns the median node, compared on the split dimension
// and the remaining nodes.
func med(split int, nodes []*Node) (*Node, []*Node) {
	if len(nodes) == 0 {
		panic("med: no nodes")
	}
	sort.Sort(nodeSorter{split, nodes})
	var m int
	for m = len(nodes) / 2; m >= 1; m-- {
		if nodes[m-1].Point[split] < nodes[m].Point[split] {
			break
		}
	}
	nodes[0], nodes[m] = nodes[m], nodes[0]
	return nodes[0], nodes[1:]
}

// A nodeSorter implements sort.Interface, sortnig the nodes
// in ascending order of their point values on the split dimension.
type nodeSorter struct {
	split int
	nodes []*Node
}

func (n nodeSorter) Len() int {
	return len(n.nodes)
}

func (n nodeSorter) Swap(i, j int) {
	n.nodes[i], n.nodes[j] = n.nodes[j], n.nodes[i]
}

func (n nodeSorter) Less(i, j int) bool {
	return n.nodes[i].Point[n.split] < n.nodes[j].Point[n.split]
}
