package main

import (
	"code.google.com/p/eaburns/ui"
	"fmt"
	"image/color"
)

func main() {
	ui.Init()
	defer ui.Deinit()

	if err := ui.OpenWindow(640, 480); err != nil {
		panic(err)
	}

	img, err := ui.LoadPng("gopher.png")
	if err != nil {
		panic(err)
	}
	defer img.Release()
	fmt.Printf("Loaded the image\n")

	font, err := ui.LoadTtf("prstartk.ttf", 12, color.RGBA{R: 0, G: 255, B: 0, A: 255})
	if err != nil {
		panic(err)
	}
	fmt.Printf("Loaded the font\n")

	ui.Clear()
	img.Draw(0, 0)
	font.Draw(200, 10, "Hello")
	font.Draw(10, 100, "World")
	font.Draw(100, 200, "Eloquent")
	ui.Flip()

	for {
	}
}
