// The perlin package has routines for generating and viewing
// Perlin noise functions.
// The implementation is based off of the one described at
// http://freespace.virgin.net/hugo.elias/models/m_perlin.htm
// with some modifications.
package perlin

import (
	"fmt"
	"math"
)

// Noise2d defines the parameters for a 2D Perlin noise function.
type Noise2d func(x, y float64) float64

// Make returns a Perlin noise function using the parameters.
// If interp is nil then cosine interpolation is used.
func Make(per, scale float64, n int, seed int64, interp func(a, b, x float64) float64) Noise2d {
	if interp == nil {
		interp = CosInterp
	}
	return func(x, y float64) float64 {
		x *= scale
		y *= scale
		tot := 0.0
		freq := 1.0
		amp := 1.0
		for i := 0; i < n; i++ {
			tot += interp2d(x*freq, y*freq, seed, interp) * amp
			amp *= per
			freq *= 2
		}
		return tot
	}
}

// interp2d returns noise for x,y interpolated from
// the given 2D noise function.
func interp2d(x, y float64, seed int64, interp func(a, b, x float64) float64) float64 {
	intx, fracx := int(x), x-math.Trunc(x)
	inty, fracy := int(y), y-math.Trunc(y)

	v1 := smooth2d(intx, inty, seed)
	v2 := smooth2d(intx+1, inty, seed)
	i1 := interp(v1, v2, fracx)

	v3 := smooth2d(intx, inty+1, seed)
	v4 := smooth2d(intx+1, inty+1, seed)
	i2 := interp(v3, v4, fracx)

	return interp(i1, i2, fracy)
}

// LinearInterp linearly interpolates the value x that is
// a factor of the distance between a and b.
func LinearInterp(a, b, x float64) float64 {
	v := a*(1-x) + b*x
	if v < 0 {
		panic(fmt.Sprintf("%g*(1-%g) + %g*%g < 0", a, x, b, x))
	}
	return v
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

// smooth2d returns smoothed noise for the x,y coordinate.
func smooth2d(x, y int, seed int64) float64 {
	s := 0.0
	for _, d := range sides {
		s += noise2d(x+d.dx, y+d.dy, seed)
	}
	c := 0.0
	for _, d := range corners {
		c += noise2d(x+d.dx, y+d.dy, seed)
	}
	return noise2d(x, y, seed)/4 + s/8 + c/16
}

// noise1d returns an integer between ­1 and 1.  Each value
// n will return the same integer each time.
func noise1d(n int, seed int64) float64 {
	m := (int32(n) << 13) ^ int32(n) + int32(seed*7)
	return 1 - float64((m*(m*m*15731+789221)+1376312589)&0x7fffffff)/1073741824
}

// noise2d returns an integer between ­1 and 1.  Each pair
// x, y will return the same integer each time.
func noise2d(x, y int, seed int64) float64 {
	return noise1d(x+y*57, seed)
}
