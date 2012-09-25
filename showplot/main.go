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
	p := makePlot()
	p.Draw(da)
}

// makePlot returns one of the example plots.
// In this particluar case, it's the example
// bar chart.
func makePlot() *plot.Plot {
	groupA := plotter.Values{20, 35, 30, 35, 27}
	groupB := plotter.Values{25, 32, 34, 20, 25}
	groupC := plotter.Values{12, 28, 15, 21, 8}

	p, err := plot.New()
	if err != nil {
		panic(err)
	}
	p.Title.Text = "Bar chart"
	p.Y.Label.Text = "Heights"

	w := vg.Points(20)

	barsA := plotter.NewBarChart(groupA, w)
	barsA.Color = color.RGBA{R: 255, G: 67, B: 67, A: 255}
	barsA.Offset = -w

	barsB := plotter.NewBarChart(groupB, w)
	barsB.Color = color.RGBA{R: 67, G: 255, B: 67, A: 255}

	barsC := plotter.NewBarChart(groupC, w)
	barsC.Color = color.RGBA{R: 67, G: 67, B: 255, A: 255}
	barsC.Offset = w

	p.Add(barsA, barsB, barsC)
	p.Legend.Add("Group A", barsA)
	p.Legend.Add("Group B", barsB)
	p.Legend.Add("Group C", barsC)
	p.Legend.Top = true
	p.NominalX("One", "Two", "Three", "Four", "Five")
	return p
}
