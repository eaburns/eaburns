// showplot is a tiny demo program that
// displays a plot drawn by Plotinum.
package main

import (
	"code.google.com/p/plotinum/plot"
	"code.google.com/p/plotinum/plotter"
	"code.google.com/p/plotinum/vg"
	"code.google.com/p/plotinum/vg/vgimg"
	"code.google.com/p/x-go-binding/ui/x11"
	"image/color"
	"image/draw"
	"math"
	"math/rand"
)

func main() {
	win, err := x11.NewWindow()
	if err != nil {
		panic(err)
	}

	drawPlot(win.Screen())
	win.FlushImage()

	events := win.EventChan()
	for _ = range events {
	}
}

// drawPlot draws the plot to an image.
func drawPlot(img draw.Image) {
	c, w, h := vgimg.NewImage(img)
	da := plot.NewDrawArea(c, w, h)

	left := *da
	left.Size.X /= 2
	hist := histPlot()
	hist.Draw(left)

	right := *da
	right.Min.X = left.Min.X + left.Size.X
	right.Size.X /= 2
	lines := linesPlot()
	lines.Draw(right)
}

func histPlot() *plot.Plot {
	// Draw some random values from the standard
	// normal distribution.
	rand.Seed(int64(0))
	v := make(plotter.Values, 10000)
	for i := range v {
		v[i] = rand.NormFloat64()
	}

	// Make a plot and set its title.
	p, err := plot.New()
	if err != nil {
		panic(err)
	}
	p.Title.Text = "Histogram"

	// Create a histogram of our values drawn
	// from the standard normal.
	h := plotter.NewHist(v, 16)
	// Normalize the area under the histogram to
	// sum to one.
	h.Normalize(1)
	p.Add(h)

	// The normal distribution function
	norm := plotter.NewFunction(stdNorm)
	norm.Color = color.RGBA{R: 255, A: 255}
	norm.Width = vg.Points(2)
	p.Add(norm)
	return p
}

// stdNorm returns the probability of drawing a
// value from a standard normal distribution.
func stdNorm(x float64) float64 {
	const sigma = 1.0
	const mu = 0.0
	const root2π = 2.50662827459517818309
	return 1.0 / (sigma * root2π) * math.Exp(-((x-mu)*(x-mu))/(2*sigma*sigma))
}

func linesPlot() *plot.Plot {
	// Get some random points
	rand.Seed(int64(0))
	n := 15
	scatterData := randomPoints(n)
	lineData := randomPoints(n)
	linePointsData := randomPoints(n)

	// Create a new plot, set its title and
	// axis labels.
	p, err := plot.New()
	if err != nil {
		panic(err)
	}
	p.Title.Text = "Points Example"
	p.X.Label.Text = "X"
	p.Y.Label.Text = "Y"
	// Draw a grid behind the data
	p.Add(plotter.NewGrid())

	// Make a scatter plotter and set its style.
	s := plotter.NewScatter(scatterData)
	s.GlyphStyle.Color = color.RGBA{R: 255, B: 128, A: 255}

	// Make a line plotter and set its style.
	l := plotter.NewLine(lineData)
	l.LineStyle.Width = vg.Points(1)
	l.LineStyle.Dashes = []vg.Length{vg.Points(5), vg.Points(5)}
	l.LineStyle.Color = color.RGBA{B: 255, A: 255}

	// Make a line plotter with points and set its style.
	lpLine, lpPoints := plotter.NewLinePoints(linePointsData)
	lpLine.Color = color.RGBA{G: 255, A: 255}
	lpPoints.Shape = plot.PyramidGlyph{}
	lpPoints.Color = color.RGBA{R: 255, A: 255}

	// Add the plotters to the plot, with a legend
	// entry for each
	p.Add(s, l, lpLine, lpPoints)
	p.Legend.Add("scatter", s)
	p.Legend.Add("line", l)
	p.Legend.Add("line points", lpLine, lpPoints)
	return p
}

// randomPoints returns some random x, y points.
func randomPoints(n int) plotter.XYs {
	pts := make(plotter.XYs, n)
	for i := range pts {
		if i == 0 {
			pts[i].X = rand.Float64()
		} else {
			pts[i].X = pts[i-1].X + rand.Float64()
		}
		pts[i].Y = pts[i].X + 10*rand.Float64()
	}
	return pts
}
