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
		{Point{-1.0, 1.0, -0.2}, Color{1.0, 1.0, 1.0}},
		{Point{1.0, 1.0, -0.2}, Color{0.0, 0.0, 1.0}},
	},

	Objects: []Object{
		Solid{
			C:     Color{1.0, 0.0, 0.0},
			Shine: 3.0,
			Shape: Sphere{Point{0.25, 0.20, -0.5}, 0.25},
		},
		Solid{
			C:     Color{0.0, 1.0, 0.0},
			Shine: 3.0,
			Shape: Sphere{Point{0.75, 0.75, -0.5}, 0.25},
		},
	},
}

var eye = Point{0.5, 0.5, 1.0}

func main() {
	img := image.NewRGBA(image.Rect(0, 0, 480, 480))
	b := img.Bounds()
	for x := b.Min.X; x < b.Max.X; x++ {
		for y := b.Min.Y; y < b.Max.Y; y++ {
			px := img2World(b, float64(x), float64(y))
			dir := px.Minus(eye)
			pxDist := dir.Dot(dir)

			hit, ok := scene.Hit(eye, dir)
			if !ok {
				img.Set(x, y, color.Black)
			} else if hit.Distance > pxDist {
				c := hit.Object.Color(scene, hit, 0)
				img.Set(x, y, c.ImageColor())
			}
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
func img2World(b image.Rectangle, x, y float64) Point {
	w := float64(b.Max.X - b.Min.X)
	h := float64(b.Max.Y - b.Min.Y)
	return Point{
		(x - float64(b.Min.X)) / w,
		1.0 - (y-float64(b.Min.Y))/h,
		0.0,
	}
}

// Point is a point in 3D space.
type Point [3]float64

// Dot returns the dot-product of two points.
func (a Point) Dot(b Point) float64 {
	return a[0]*b[0] + a[1]*b[1] + a[2]*b[2]
}

// Scale returns a point with all of its
// components scaled by a constant
// factor.
func (a Point) Scale(f float64) Point {
	return Point{a[0] * f, a[1] * f, a[2] * f}
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
func (a Point) Normalize() Point {
	return a.Scale(1.0 / math.Sqrt(a.Dot(a)))
}

// Color is an RGB color.
type Color Point

func (c Color) ImageColor() color.Color {
	r := uint8(math.Min(1.0, c[0]) * 255)
	g := uint8(math.Min(1.0, c[1]) * 255)
	b := uint8(math.Min(1.0, c[2]) * 255)
	return color.RGBA{R: r, G: g, B: b, A: 255}
}
