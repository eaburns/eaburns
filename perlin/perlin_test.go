package perlin

import (
	"testing"
	"testing/quick"
	"time"
)

// TestNoise1d checks that the noise1d function never returns a value
// out of its supposid range of ­1 and 1.
func TestNoise1d(t *testing.T) {
	f := func(x int) bool {
		y := noise1d(x, 0)
		return y <= 1 && y >= -1
	}
	if err := quick.Check(f, &quick.Config{MaxCount: 10000}); err != nil {
		t.Error(err)
	}
}

// TestNoise2d checks that the noise2d function never returns a value
// out of its supposid range of ­1 and 1.
func TestNoise2d(t *testing.T) {
	f := func(x, y int) bool {
		z := noise2d(x, y, 0)
		return z <= 1 && z >= -1
	}
	if err := quick.Check(f, &quick.Config{MaxCount: 10000}); err != nil {
		t.Error(err)
	}
}

// TestSmooth2d checks that the smooth2d function
// never returns a value out of its supposid range of ­1 and 1.
func TestSmooth2d(t *testing.T) {
	f := func(x, y int) bool {
		z := smooth2d(x, y, 0)
		return z <= 1 && z >= -1
	}
	if err := quick.Check(f, &quick.Config{MaxCount: 10000}); err != nil {
		t.Error(err)
	}
}

// BenchmarkNoise1d benchmarks the noise1d function.
func BenchmarkNoise1d(b *testing.B) {
	for i := 0; i < b.N; i++ {
		noise1d(i, 0)
	}
}

// BenchmarkNoise2d benchmarks the noise1d function.
func BenchmarkNoise2d(b *testing.B) {
	for i := 0; i < b.N; i++ {
		noise2d(i, i, 0)
	}
}

// BenchmarkSmooth2d benchmarks the noise1d function.
func BenchmarkSmooth2d(b *testing.B) {
	for i := 0; i < b.N; i++ {
		smooth2d(i, i, 0)
	}
}

// BenchmarkNoise2dCos benchmarks the 2D Perlin noise function
// using cosine interpolation.
func BenchmarkNoise2dCos(b *testing.B) {
	n := Make(0.001, 0.02, 1, time.Now().UnixNano(), nil)
	for i := 0; i < b.N; i++ {
		n(float64(i), float64(i))
	}
}

// BenchmarkNoise2dLin benchmarks the 2D Perlin noise function
// using linear interpolation.
func BenchmarkNoise2dLin(b *testing.B) {
	n := Make(0.001, 0.02, 1, time.Now().UnixNano(), LinearInterp)
	for i := 0; i < b.N; i++ {
		n(float64(i), float64(i))
	}
}
