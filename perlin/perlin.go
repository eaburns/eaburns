// The perlin package has routines for generating and viewing
// Perlin noise functions.
// The implementation is based off of the one described here:
// 	http://freespace.virgin.net/hugo.elias/models/m_perlin.htm
// with some modifications.
package perlin

import (
	"image"
	"image/color"
	"image/png"
	"math"
	"os"
)

// Noise2d defines the parameters for a 2D Perlin noise function.
type Noise2d struct {
	Persistance, Scale float64
	Octaves            int
	Seed               int64
	Interp             func(a, b, x float64) float64
	Noise              func(int, int, int64) float64
}

// New returns a new Perlin noise function with the given
// parameters: persistance, scale, number of octaves, and seed.
// The default uses cosine interpolation.
func New(persist, scale float64, noct int, seed int64) *Noise2d {
	return &Noise2d{
		Persistance: persist,
		Scale:       scale,
		Octaves:     noct,
		Seed:        seed,
		Interp:      CosInterp,
		Noise:       noise2d,
	}
}

// At returns the Perlin noise value at coordinate x,y
func (n *Noise2d) At(x, y float64) float64 {
	x *= n.Scale
	y *= n.Scale
	tot := 0.0
	freq := 1.0
	amp := 1.0
	for i := 0; i < n.Octaves; i++ {
		tot += n.interp2d(x*freq, y*freq) * amp
		amp *= n.Persistance
		freq *= 2
	}
	return tot
}

// A NoiseImage implements the image.Image interface using
// a Perlin noise function.
type NoiseImage Noise2d

func (n *NoiseImage) At(x, y int) color.Color {
	noise := (*Noise2d)(n)
	f := noise.At(float64(x), float64(y))
	switch {
	case f > 1:
		f = 1
	case f < 0:
		f = 0
	}
	return color.Gray{Y: uint8(255 * f)}
}

// Bounds returns the bounds on the image.
func (n *NoiseImage) Bounds() image.Rectangle {
	return image.Rect(0, 0, 500, 500)
}

// ColorModel returns the image's color model.
func (n *NoiseImage) ColorModel() color.Model {
	return color.GrayModel
}

// SavePng saves the noise to a PNG file.
func (n *NoiseImage) SavePng(path string) error {
	f, err := os.Create(path)
	if err != nil {
		return err
	}
	defer f.Close()

	if err := png.Encode(f, n); err != nil {
		return err
	}

	return nil
}

// interp2d returns noise for x,y interpolated from
// the given 2D noise function.
func (n *Noise2d) interp2d(x, y float64) float64 {
	intx, fracx := int(x), x-math.Trunc(x)
	inty, fracy := int(y), y-math.Trunc(y)

	v1 := n.smooth2d(intx, inty)
	v2 := n.smooth2d(intx+1, inty)
	i1 := n.Interp(v1, v2, fracx)

	v3 := n.smooth2d(intx, inty+1)
	v4 := n.smooth2d(intx+1, inty+1)
	i2 := n.Interp(v3, v4, fracx)

	return n.Interp(i1, i2, fracy)
}

// LinearInterp linearly interpolates the value x that is
// a factor of the distance between a and b.
func LinearInterp(a, b, x float64) float64 {
	return a*(1-x) + b*x
}

// CosInterp cosine interpolates the value x that is
// a factor of the distance between a and b.
//
// Cosine interpolation is slower than linear interpolation
// but it is also much smoother.
func CosInterp(a, b, x float64) float64 {
	f := (1 - math.Cos(x*math.Pi)) * .5
	return a*(1-f) + b*f
}

var (
	sides   = [...]struct{ dx, dy int }{{1, 0}, {-1, 0}, {0, 1}, {0, -1}}
	corners = [...]struct{ dx, dy int }{{1, 1}, {-1, 1}, {1, -1}, {-1, -1}}
)

// smooth2d returns smoothed noise for the x,y
// coordinate using the given underlying noise function.
// If n.W > 0 then the resulting noise should line up in the
// horizontal direction (i.e., the left and right edges should
// be 'tileable'), and likewise with n.H and the vertical direction.
func (n *Noise2d) smooth2d(x, y int) float64 {
	s := 0.0
	for _, d := range sides {
		s += n.Noise(x+d.dx, y+d.dy, n.Seed)
	}
	c := 0.0
	for _, d := range corners {
		c += n.Noise(x+d.dx, y+d.dy, n.Seed)
	}
	return n.Noise(x, y, n.Seed)/4 + s/8 + c/16
}

// noise1d returns an integer between 0 and 1.  Each value
// n will return the same integer each time.
func noise1d(n int, seed int64) float64 {
	m := (int32(n) << 13) ^ int32(n) + int32(seed*7)

	// 2147483648 is two times 1073741824 (from the aformentioned
	// website) this change moves the result to the range 0—1, not ­1—1,
	// the remaining values are directly from the website.
	return float64((m*(m*m*15731+789221)+1376312589)&0x7fffffff) / 2147483648
}

// noise2d returns an integer between 0 and 1.  Each pair
// x, y will return the same integer each time.
func noise2d(x, y int, seed int64) float64 {
	return noise1d(x+y*57, seed)
}
