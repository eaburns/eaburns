package main

import (
	"os"
	"fmt"
	"code.google.com/p/eaburns/ui"
)

func init() {
	ui.Init()
}

func main() {
	defer ui.Deinit()

	if err := ui.OpenWindow(200,200); err != nil {
		panic(err)
	}

	f, err := os.Open("gopher.png")
	if err != nil {
		panic(err)
	}
	defer f.Close()

	img, err := ui.ReadImage(f)
	if err != nil {
		panic(err)
	}
	fmt.Printf("Loaded\n");
	img.W = 64
	img.H = 64

	ui.Clear()
	img.Draw(0, 0)
	ui.Flip()

	for { }
}
