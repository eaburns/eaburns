package main

import (
	"code.google.com/p/eaburns/ui"
	"fmt"
)

func main() {
	ui.Init()
	defer ui.Deinit()

	if err := ui.OpenWindow(200, 200); err != nil {
		panic(err)
	}

	img, err := ui.LoadPng("gopher.png")
	if err != nil {
		panic(err)
	}
	defer img.Release()

	fmt.Printf("Loaded the image\n")
	// Resize the image
	img.Width, img.Height = 64, 64

	ui.Clear()
	img.Draw(0, 0)
	ui.Flip()

	for {
	}
}
