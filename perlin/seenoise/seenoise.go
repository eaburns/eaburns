// The seenoise package draws an PNG image of
// a random Perlin function with a given set of
// parameters.
package main

import (
	"code.google.com/p/eaburns/perlin"
	"flag"
	"time"
	"image"
	"image/color"
	"image/png"
	"math"
	"os"
	"fmt"
)

var (
	width = flag.Int("w", 500, "The width of the image")
	height = flag.Int("h", 500, "The height of the image")
	seed = flag.Int64("seed", 0, "The seed, (0 == use time)")
	persist = flag.Float64("p", 0.001, "The persistance (less small variations?)")
	scale = flag.Float64("s", 0.25, "The scale (smaller zooms in to the noise)")
	noct = flag.Int("n", 4, "Numer of octaves (number of noise functions added together)")
	linInterp = flag.Bool("l", false, "Use linear instead of cosine interpolation")
	outpath = flag.String("o", "noise.png", "The output file path")
)

func main() {
	flag.Parse()

	if *seed == 0 {
		*seed = time.Now().UnixNano()
		fmt.Println("seed", *seed)
	}

	interp := perlin.CosInterp
	if *linInterp {
		interp = perlin.LinearInterp
	}

	noise := perlin.Make(*persist, *scale, *noct, *seed, interp)
	img := makeNoiseImg(*width, *height, noise)
	
	f, err := os.Create(*outpath)
	if err != nil {
		panic(err)
	}
	defer f.Close()

	if err := png.Encode(f, img); err != nil {
		panic(err)
	}
}

type noiseImg struct{
	w, h int
	pts []float64
}

// makeNoiseImg returns a noise image.
func makeNoiseImg(w, h int, noise func(float64,float64)float64) noiseImg {
	min, max := math.Inf(1), 0.0
	pts := make([]float64, w*h)
	for x := 0; x < w; x++ {
		for y := 0; y < h; y++ {
			n := noise(float64(x), float64(y))
			pts[x*h + y] = n
			if n > max {
				max = n
			}
			if n < min {
				min = n
			}
		}
	}
	for i := range pts {
		pts[i] = (pts[i]-min)/(max-min)
	}
	return noiseImg{ w, h, pts }
}

func (n noiseImg) At(x, y int) color.Color {
	return color.Gray{ uint8(255*n.pts[x*n.h + y]) }
}

func (n noiseImg) ColorModel() color.Model {
	return color.GrayModel
}

func (n noiseImg) Bounds() image.Rectangle {
	return image.Rect(0, 0, n.w, n.h)
}