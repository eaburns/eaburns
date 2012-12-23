package kdtree

import (
	"math/rand"
	"testing"
)

// RadiusMax is the maximum radius for InRange benchmarks.
const radiusMax = 0.1

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
	var t *T
	for i := range pts {
		t = t.Insert(&T{Point: pts[i]})
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
	var t *T
	for i := 0; i < b.N; i++ {
		for i := range pts {
			t = t.Insert(&T{Point: pts[i]})
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
	nodes := make([]T, sz)
	nodeps := make([]*T, sz)
	for i := range nodes {
		nodes[i].Point = pts[i]
		nodeps[i] = &nodes[i]
	}

	for i := 0; i < b.N; i++ {
		New(nodeps)
	}

}

// BenchmarkInRange1000 benchmarks the InRange function with
// a tree containing 1000 nodes.
func BenchmarkInRange1000(b *testing.B) {
	inRangeSz(1000, b)
}

// inRangeSz benchmarks InRange function on a tree with the given
// number of nodes.
func inRangeSz(sz int, b *testing.B) {
	b.StopTimer()
	nodes := make([]T, sz)
	nodeps := make([]*T, sz)
	for i := range nodes {
		for j := range nodes[i].Point {
			nodes[i].Point[j] = rand.Float64()
		}
		nodeps[i] = &nodes[i]
	}
	tree := New(nodeps)

	points := make([]Point, b.N)
	for i := range points {
		for j := range points[i] {
			points[i][j] = rand.Float64()
		}
	}
	rs := make([]float64, b.N)
	for i := range rs {
		rs[i] = rand.Float64()
	}

	b.StartTimer()
	for i, pt := range points {
		tree.InRange(pt, rs[i]*radiusMax)
	}
}

// BenchmarkInRangeSlice1000 benchmarks the InRangeSlice
// function with a tree containing 1000 nodes.
func BenchmarkInRangeSlice1000(b *testing.B) {
	inRangeSliceSz(1000, b)
}

// inRangeSliceSz benchmarks InRangeSlice function on a tree
// with the given number of nodes.
func inRangeSliceSz(sz int, b *testing.B) {
	b.StopTimer()
	nodes := make([]T, sz)
	nodeps := make([]*T, sz)
	for i := range nodes {
		for j := range nodes[i].Point {
			nodes[i].Point[j] = rand.Float64()
		}
		nodeps[i] = &nodes[i]
	}
	tree := New(nodeps)

	points := make([]Point, b.N)
	for i := range points {
		for j := range points[i] {
			points[i][j] = rand.Float64()
		}
	}
	rs := make([]float64, b.N)
	for i := range rs {
		rs[i] = rand.Float64()
	}

	pool := make([]*T, 0, sz)

	b.StartTimer()
	for i, pt := range points {
		tree.InRangeSlice(pt, rs[i]*radiusMax, pool[:0])
	}
}

// BenchmarkInRangeLiner1000 benchmarks computing the in range
// nodes via a linear scan.
func BenchmarkInRangeLinear1000(b *testing.B) {
	inRangeLinearSz(1000, b)
}

// inRangeLinearSz benchmarks computing in range nodes using
// a linear scan of the given number of nodes.
func inRangeLinearSz(sz int, b *testing.B) {
	b.StopTimer()
	nodes := make([]T, sz)
	for i := range nodes {
		for j := range nodes[i].Point {
			nodes[i].Point[j] = rand.Float64()
		}
	}

	points := make([]Point, b.N)
	for i := range points {
		for j := range points[i] {
			points[i][j] = rand.Float64()
		}
	}
	rs := make([]float64, b.N)
	for i := range rs {
		rs[i] = rand.Float64() * radiusMax
	}

	local := make([]*T, 0, sz)

	b.StartTimer()
	for i, pt := range points {
		local = local[:0]
		rr := rs[i] * rs[i]
		for i := range nodes {
			if nodes[i].Point.sqDist(&pt) < rr {
				local = append(local, &nodes[i])
			}
		}
	}
}
