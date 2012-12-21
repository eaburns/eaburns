package kdtree

// K is the number of dimensions of the stored points.
const K = 2

type Point [K]float64

// T is a node in a KD-tree.   A *T is the root of a KD-tree, and nil is the
// empty tree.
type T struct {
	pt          Point
	split       int
	data        interface{}
	left, right *T
}

// Insert inserts data associated with a given point into the KD tree.
func (t *T) Insert(pt Point, data interface{}) *T {
	return t.insert(0, pt, data)
}

func (t *T) insert(depth int, pt Point, data interface{}) *T {
	if t == nil {
		return &T{pt: pt, split: split(depth), data: data}
	}
	if pt[t.split] < t.pt[t.split] {
		t.left = t.left.insert(depth+1, pt, data)
	} else {
		t.right = t.right.insert(depth+1, pt, data)
	}
	return t
}

// Split returns the splitting dimension from the depth.
func split(d int) int {
	return (d + 1) % K
}
