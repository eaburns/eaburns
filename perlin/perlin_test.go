package perlin

import (
	"testing"
	"testing/quick"
	"time"
)

// TestSavePng tests saving to a PNG file.
func TestSavePng(t *testing.T) {
	n := New(0.001, 0.02, 1, time.Now().UnixNano())
	if err := (*NoiseImage)(n).SavePng("test.png"); err != nil {
		t.Error(err)
	}
}

// TestNoise1d checks that the noise1d function never returns a value
// out of its supposid range of 0 and 1.
func TestNoise1d(t *testing.T) {
	f := func(x int) bool {
		y := noise1d(x, 0)
		return y <= 1.0 && y >= 0.0
	}
	if err := quick.Check(f, &quick.Config{MaxCount: 10000}); err != nil {
		t.Error(err)
	}
}

// TestNoise2d checks that the noise2d function never returns a value
// out of its supposid range of 0 and 1.
func TestNoise2d(t *testing.T) {
	f := func(x, y int) bool {
		z := noise2d(x, y, 0)
		return z <= 1.0 && z >= 0.0
	}
	if err := quick.Check(f, &quick.Config{MaxCount: 10000}); err != nil {
		t.Error(err)
	}
}

// TestSmoothNoise2d checks that the smoothNoise2d function
// never returns a value out of its supposid range of 0 and 1.
func TestSmoothedNoise2d(t *testing.T) {
	n := New(0, 0, 0, 0)
	f := func(x, y int) bool {
		z := n.smooth2d(x, y)
		return z <= 1.0 && z >= 0.0
	}
	if err := quick.Check(f, &quick.Config{MaxCount: 10000}); err != nil {
		t.Error(err)
	}
}
