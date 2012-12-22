package kdtree

import (
	"math/rand"
	"testing"
)

// BenchmarkInsert benchmarks insertions into an initially empty tree.
func BenchmarkInsert(b *testing.B) {
	b.StopTimer()
	pts := make([]Point, b.N)
	for i := range pts {
		for j := range pts[i] {
			pts[i][j] = rand.Float64()
		}
	}

	b.StartTimer()
	var t Root
	for i := range pts {
		t.Insert(pts[i], nil)
	}
}

// BenchmarkInsert1000 benchmarks 1000 insertions into an empty tree.
func BenchmarkInsert1000(b *testing.B) {
	insertSz(1000, b)
}

// InsertSz benchmarks inserting sz nodes into an empty tree.
func insertSz(sz int, b *testing.B) {
	b.StopTimer()
	pts := make([]Point, sz)
	for i := range pts {
		for j := range pts[i] {
			pts[i][j] = rand.Float64()
		}
	}

	b.StartTimer()
	var t Root
	for i := 0; i < b.N; i++ {
		for i := range pts {
			t.Insert(pts[i], nil)
		}
	}

}

// BenchmarkMake1000 benchmarks Make with 1000 nodes.
func BenchmarkMake1000(b *testing.B) {
	makeSz(1000, b)
}

// MakeSz benchmarks Make with a given number of nodes.
// The time includes allocating the nodes.
func makeSz(sz int, b *testing.B) {
	b.StopTimer()
	pts := make([]Point, sz)
	for i := range pts {
		for j := range pts[i] {
			pts[i][j] = rand.Float64()
		}
	}

	b.StartTimer()
	nodes := make([]Node, sz)
	nodeps := make([]*Node, sz)
	for i := range nodes {
		nodes[i].Point = pts[i]
		nodeps[i] = &nodes[i]
	}

	for i := 0; i < b.N; i++ {
		Make(nodeps)
	}

}
