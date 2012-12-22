package kdtree

import (
	"math/rand"
)

// K is the number of dimensions of the stored points.
const K = 2

// Point is a location in K-dimensional space.
type Point [K]float64

// Root is the root of a K-D tree.  The zero-value is an empty tree.
type Root struct {
	node *Node
}

// Make returns a new K-D tree built using the given nodes.
func Make(nodes []*Node) Root {
	return Root{ buildTree(0, nodes) }
}

// Insert inserts data associated with a given point into the KD tree.
func (r *Root) Insert(Point Point, Data interface{}) {
	r.node = r.node.insert(0, Point, Data)
}

// A Node is a node in the K-D tree, pairing a point in K-dimensional
// space with a value.
type Node struct {
	// Point is the K-dimensional point associated with the
	// data of this node.
	Point
	// Data is auxiliary data associated with the point of this node.
	Data        interface{}

	split       int
	left, right *Node
}

// Insert inserts the point, data pair beneath the given node, returning
// a new node rooting the new subtree.
func (t *Node) insert(depth int, Point Point, Data interface{}) *Node {
	if t == nil {
		return &Node{Point: Point, split: depth % K, Data: Data}
	}
	if Point[t.split] < t.Point[t.split] {
		t.left = t.left.insert(depth+1, Point, Data)
	} else {
		t.right = t.right.insert(depth+1, Point, Data)
	}
	return t
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
	cur, nodes := med3(split, nodes)
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

// Med3 gets three random nodes in the slice, removes the node
// that has the median value of the three on the splitting dimension,
// and returns the median node and the rest of the slice.
func med3(split int, nodes []*Node) (*Node, []*Node) {
	switch len(nodes) {
	case 0:
		panic("med3: no nodes")
	case 1:
		return nodes[0], []*Node{}
	case 2:
		return nodes[0], nodes[1:]
	}
	inds := [3]int{
		rand.Intn(len(nodes)),
		rand.Intn(len(nodes)),
		rand.Intn(len(nodes)),
	}
	if nodes[inds[1]].Point[split] < nodes[inds[0]].Point[split] {
		inds[0], inds[1] = inds[1], inds[0]
	}
	if nodes[inds[2]].Point[split] < nodes[inds[1]].Point[split] {
		inds[1], inds[2] = inds[2], inds[1]
	}
	if nodes[inds[1]].Point[split] < nodes[inds[0]].Point[split] {
		inds[0], inds[1] = inds[1], inds[0]
	}
	med := inds[2]

	nodes[0], nodes[med] = nodes[med], nodes[0]
	return nodes[0], nodes[1:]
}
