// showplot is a tiny demo program that
// displays a plot drawn by Plotinum.
package main

import (
	"code.google.com/p/plotinum/plot"
	"code.google.com/p/plotinum/plotter"
	"code.google.com/p/plotinum/vg"
	"code.google.com/p/plotinum/vg/vgimg"
	"code.google.com/p/x-go-binding/ui"
	"code.google.com/p/x-go-binding/ui/x11"
	"fmt"
	"image/color"
	"image/draw"
	"math"
	"math/rand"
	"os"
	"runtime/pprof"
)

type plots [2]struct {
	plot     *plot.Plot
	dataArea plot.DrawArea
}

var (
	cpuProfile = "cpu.prof"
	memProfile = "mem.prof"
)

var ps plots

var font vg.Font

func main() {
	ps[0].plot = linesPlot()
	ps[1].plot = histPlot()

	var err error
	font, err = vg.MakeFont("Times-Roman", vg.Points(12))
	if err != nil {
		panic(err)
	}

	win, err := x11.NewWindow()
	if err != nil {
		panic(err)
	}

	drawPlots(win.Screen())
	win.FlushImage()

	if cpuProfile != "" {
		f, err := os.Create(cpuProfile)
		if err != nil {
			panic(err)
		}
		pprof.StartCPUProfile(f)
		defer pprof.StopCPUProfile()
	}
	if memProfile != "" {
		f, err := os.Create(memProfile)
		if err != nil {
			panic(err)
		}
		pprof.WriteHeapProfile(f)
		f.Close()
	}

	events := win.EventChan()
	for ev := range events {
		if m, ok := ev.(ui.MouseEvent); ok && m.Buttons == 1 {
			winHeight := 600 // hard-coded for ui/x11…
			p, x, y := dataCoord(m.Loc.X, winHeight-m.Loc.Y)
			if p >= 0 {
				str := fmt.Sprintf("plot: %d, coord: %g, %g\n", p, x, y)
				crosshair(win.Screen(), m.Loc.X, winHeight-m.Loc.Y, str)
				win.FlushImage()
			}
		}
	}
}

// drawPlots draws the plots to an image.
func drawPlots(img draw.Image) {
	c := vgimg.NewImage(img)
	da := plot.MakeDrawArea(c)

	textAreaSize := vg.Points(12)
	da.Min.Y += textAreaSize
	da.Size.Y -= textAreaSize

	left := da
	left.Size.X /= 2
	ps[0].plot.Draw(left)
	ps[0].dataArea = ps[0].plot.DataDrawArea(left)

	right := da
	right.Min.X = left.Min.X + left.Size.X
	right.Size.X /= 2
	ps[1].plot.Draw(right)
	ps[1].dataArea = ps[1].plot.DataDrawArea(right)
}

// crosshair draws a plus at the given point.
func crosshair(img draw.Image, x, y int, str string) {
	c := vgimg.NewImage(img)

	// drawPlots here because NewImage
	// clears the canvas.  Instead, the canvas
	// should just be stored instead of being
	// recreated at each redraw.
	drawPlots(img)

	c.SetColor(color.RGBA{R: 255, A: 255})

	xc := vg.Inches(float64(x) / c.DPI())
	yc := vg.Inches(float64(y) / c.DPI())
	radius := vg.Points(5)

	var p vg.Path
	p.Move(xc-radius, yc)
	p.Line(xc+radius, yc)
	c.Stroke(p)

	p = vg.Path{}
	p.Move(xc, yc+radius)
	p.Line(xc, yc-radius)
	c.Stroke(p)

	c.SetColor(color.Black)
	c.FillString(font, vg.Length(0), vg.Length(0), str)
}

// dataCoord returns the plot number and data
// coordinate of a screen coordinate.  A negative
// plot numbers means that the screen coordinate
// is not in the data area of any plot.
func dataCoord(x, y int) (int, float64, float64) {
	dpi := ps[0].dataArea.DPI()
	pt := plot.Point{
		X: vg.Inches(float64(x) / dpi),
		Y: vg.Inches(float64(y) / dpi),
	}
	for i, p := range ps {
		if p.dataArea.Contains(pt) {
			da := p.dataArea
			x := float64((pt.X - da.Min.X) / (da.Max().X - da.Min.X))
			x *= (p.plot.X.Max - p.plot.X.Min)
			x += p.plot.X.Min

			y := float64((pt.Y - da.Min.Y) / (da.Max().Y - da.Min.Y))
			y *= (p.plot.Y.Max - p.plot.Y.Min)
			y += p.plot.Y.Min
			return i, x, y
		}
	}
	return -1, 0, 0
}

func histPlot() *plot.Plot {
	// Draw some random values from the standard
	// normal distribution.
	rand.Seed(int64(0))
	v := make(plotter.Values, 1000)
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
	h, err := plotter.NewHist(v, 16)
	if err != nil {
		panic(err)
	}
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
	n := 10
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
	s, err := plotter.NewScatter(scatterData)
	if err != nil {
		panic(err)
	}
	s.GlyphStyle.Color = color.RGBA{R: 255, B: 128, A: 255}

	// Make a line plotter and set its style.
	l, err := plotter.NewLine(lineData)
	if err != nil {
		panic(err)
	}
	l.LineStyle.Width = vg.Points(1)
	l.LineStyle.Dashes = []vg.Length{vg.Points(5), vg.Points(5)}
	l.LineStyle.Color = color.RGBA{B: 255, A: 255}

	// Make a line plotter with points and set its style.
	lpLine, lpPoints, err := plotter.NewLinePoints(linePointsData)
	if err != nil {
		panic(err)
	}
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
