package kdtree

import (
	"fmt"
	"math/rand"
)

// Generate random points in the unit square, and prints all points
// within a radius of 0.25 from the origin.
func ExampleT_InRange() {
	// Make a K-D tree of random points.
	const N = 1000
	nodes := make([]*T, N)
	for i := range nodes {
		nodes[i] = new(T)
		for j := range nodes[i].Point {
			nodes[i].Point[j] = rand.Float64()
		}
	}
	tree := New(nodes)

	rng := tree.InRange(Point{0, 0}, 0.25, make([]*T, 0, N))
	fmt.Println(rng)
}
