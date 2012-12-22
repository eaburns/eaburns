package kdtree

import (
	"fmt"
	"math/rand"
)

// Generates 1000 random points on the unit square, and prints all points
// within a radius of 0.25 from the origin.
func ExampleInRange() {
	const N = 1000
	nodes := make([]*Node, N)
	for i := range nodes {
		nodes[i] = new(Node)
		for j := range nodes[i].Point {
			nodes[i].Point[j] = rand.Float64()
		}
	}

	tree := Make(nodes)
	rng := tree.InRange(Point{0, 0}, 0.25)
	fmt.Println(rng)
}

// Generates 1000 random points on the unit square, and prints all points
// within a radius of 0.25 from the origin.
func ExampleInRangeSlice() {
	const N = 1000
	nodes := make([]*Node, N)
	for i := range nodes {
		nodes[i] = new(Node)
		for j := range nodes[i].Point {
			nodes[i].Point[j] = rand.Float64()
		}
	}
	tree := Make(nodes)

	pool := make([]Node, N)
	nodes = make([]*Node, N)
	for i := range pool {
		nodes[i] = &pool[i]
	}

	rng := tree.InRangeSlice(Point{0, 0}, 0.25, nodes)
	fmt.Println(rng)
}
