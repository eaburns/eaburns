package main

import (
	"image"
	"image/color"
	"image/png"
	"math"
	"os"
)

var scene = Scene{
	AmbientLight: Color{0.25, 0.25, 0.25},

	Lights: []Light{
		{Point{1.0, 1.0, 0.3}, Color{1.0, 1.0, 1.0}},
		{Point{-1.0, -1.0, 0.3}, Color{1.0, 1.0, 1.0}},
	},

	Objects: []Object{
		Solid{
			C:     Color{1.0, 0.0, 0.0},
			Shine: 3.0,
			Shape: Sphere{Point{0.0, 0.0, 0.0}, 0.25},
		},
		Solid{
			C:     Color{0.0, 0.0, 1.0},
			Shine: 5.0,
			Shape: Sphere{Point{0.1, 0.1, 1.0}, 0.125},
		},
		Solid{
			C:     Color{0.0, 1.0, 0.0},
			Shine: 3.0,
			Shape: Sphere{Point{0.5, 0.5, 0.0}, 0.25},
		},
	},
}

var (
	eye = Point{0, 0, 3}
	ref = Point{0, 0, 0}
	up  = Point{0, 1, 0}
)

func main() {
	img := image.NewRGBA(image.Rect(0, 0, 480, 480))
	b := img.Bounds()

	image2World := makeProjection(b)

	for x := b.Min.X; x < b.Max.X; x++ {
		for y := b.Min.Y; y < b.Max.Y; y++ {
			px := image2World(float64(x), float64(y))
			dir := px.Minus(eye)

			hit, ok := scene.Hit(eye, dir)
			c := Color{0, 0, 0}
			if ok {
				c = hit.Object.Color(scene, hit, 0)
			}
			img.Set(x, y, c.ImageColor())
		}
	}

	f, err := os.Create("image.png")
	if err != nil {
		panic(err)
	}
	defer f.Close()

	err = png.Encode(f, img)
	if err != nil {
		panic(err)
	}

}

// Img2World converts a point on the image
// to a point in the 3-dimensional world.
//
// TODO: This is not setup correctly.
func image2World(b image.Rectangle, x, y float64) Point {
	w := float64(b.Max.X - b.Min.X)
	h := float64(b.Max.Y - b.Min.Y)
	return Point{
		(x - float64(b.Min.X)) / w,
		1.0 - (y-float64(b.Min.Y))/h,
		0.0,
	}
}

func makeProjection(b image.Rectangle) func(x, y float64) Point {
	z := eye.Minus(ref).Normalize()
	x := up.Cross(z).Normalize()
	y := z.Cross(x).Normalize()
	m := [4][4]float64{
		{x[0], y[0], z[0], eye[0]},
		{x[1], y[1], z[1], eye[1]},
		{x[2], y[2], z[2], eye[2]},
	}
	clipL, clipR := -1.0, 1.0
	clipB, clipT := -1.0, 1.0
	clipN := 2.0
	dx := (clipR - clipL) / float64(b.Max.X-b.Min.X)
	height := float64(b.Max.Y - b.Min.Y)
	dy := (clipT - clipB) / height
	zv := -clipN

	return func(x, y float64) Point {
		xv := clipL + dx*x
		yv := clipB + dy*(height-y)
		var w Point
		w[0] = m[0][0]*xv + m[0][1]*yv + m[0][2]*zv + m[0][3]
		w[1] = m[1][0]*xv + m[1][1]*yv + m[1][2]*zv + m[1][3]
		w[2] = m[2][0]*xv + m[2][1]*yv + m[2][2]*zv + m[2][3]
		return w
	}
}

// Point is a point in 3D space.
type Point [3]float64

// Dot returns the dot-product of two points.
func (a Point) Dot(b Point) float64 {
	return a[0]*b[0] + a[1]*b[1] + a[2]*b[2]
}

// Cross returns the cross-product of two points.
func (a Point) Cross(b Point) Point {
	return Point{
		a[1]*b[2] - a[2]*b[1],
		a[2]*b[0] - a[0]*b[2],
		a[0]*b[1] - a[1]*b[0],
	}
}

// Scale returns a point with all of its
// components scaled by a constant
// factor.
func (p Point) Scale(f float64) Point {
	return Point{p[0] * f, p[1] * f, p[2] * f}
}

// Minus returns a new point that is the
// difference a - b.
func (a Point) Minus(b Point) Point {
	return Point{a[0] - b[0], a[1] - b[1], a[2] - b[2]}
}

// Plus returns a new point that is the
// sum a + b.
func (a Point) Plus(b Point) Point {
	return Point{a[0] + b[0], a[1] + b[1], a[2] + b[2]}
}

// Plus returns a new point that is the
// component-wise product a * b.
func (a Point) Times(b Point) Point {
	return Point{a[0] * b[0], a[1] * b[1], a[2] * b[2]}
}

// Normalize returns a normalized point.
func (p Point) Normalize() Point {
	return p.Scale(1.0 / math.Sqrt(p.Dot(p)))
}

// Color is an RGB color.
type Color Point

func (c Color) ImageColor() color.Color {
	r := uint8(math.Min(1.0, c[0]) * 255)
	g := uint8(math.Min(1.0, c[1]) * 255)
	b := uint8(math.Min(1.0, c[2]) * 255)
	return color.RGBA{R: r, G: g, B: b, A: 255}
}
