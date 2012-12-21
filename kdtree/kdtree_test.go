package kdtree

import (
	"math/rand"
	"testing"
)

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
		t = t.Insert(pts[i], nil)
	}
}