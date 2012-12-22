package kdtree

// K is the number of dimensions of the stored points.
const K = 2

// Point is a location in K-dimensional space.
type Point [K]float64

// Root is the root of a K-D tree.  The zero-value is an empty tree.
type Root struct {
	node *node
}

// Insert inserts data associated with a given point into the KD tree.
func (r *Root) Insert(pt Point, data interface{}) {
	r.node = r.node.insert(0, pt, data)
}

// A node is a node in the K-D tree, pairing a point in K-dimensional
// space with a value.
type node struct {
	pt          Point
	split       int
	data        interface{}
	left, right *node
}

// Insert inserts the point, data pair beneath the given node, returning
// a new node rooting the new subtree.
func (t *node) insert(depth int, pt Point, data interface{}) *node {
	if t == nil {
		return &node{pt: pt, split: depth%K, data: data}
	}
	if pt[t.split] < t.pt[t.split] {
		t.left = t.left.insert(depth+1, pt, data)
	} else {
		t.right = t.right.insert(depth+1, pt, data)
	}
	return t
}
