package ui

import (
	"code.google.com/p/freetype-go/freetype"
	"code.google.com/p/freetype-go/freetype/truetype"
	"errors"
	"fmt"
	"github.com/banthar/gl"
	"github.com/jteeuwen/glfw"
	"image"
	"image/color"
	"image/png"
	"image/draw"
	"io/ioutil"
	"os"
)

// Init initializes the user interface.  This must be
// called before any other functions in this package.
//
// The Deinit function should be called at the end
// of the use of this package.
func Init() error {
	return glfw.Init()
}

// Deinit de-initializes the user interface.  This
// should be the last function called in this package.
func Deinit() {
	glfw.Terminate()
}

// OpenWindow opens a new window with the given size.
func OpenWindow(w, h int) error {
	glfw.OpenWindowHint(glfw.WindowNoResize, 1)

	r, g, b := 0, 0, 0 // defaults
	a := 8             // 8-bit alpha channel
	d, s := 0, 0       // no depth or stencil buffers
	m := glfw.Windowed
	if err := glfw.OpenWindow(w, h, r, g, b, a, d, s, m); err != nil {
		return err
	}

	if gl.Init() != 0 {
		return errors.New("Failed to initialize OpenGL")
	}

	gl.Enable(gl.TEXTURE_2D)
	gl.Enable(gl.BLEND)
	gl.BlendFunc(gl.SRC_ALPHA, gl.ONE_MINUS_SRC_ALPHA)
	gl.ClearColor(0.0, 0.0, 0.0, 0.0)
	gl.MatrixMode(gl.PROJECTION)
	gl.LoadIdentity()
	gl.Ortho(0, float64(w), 0, float64(-h), -1, 1)
	gl.MatrixMode(gl.MODELVIEW)
	gl.LoadIdentity()
	gl.Translated(0, float64(-h), 0)
	return nil
}

// Flip flips the front and back buffers.
func Flip() {
	glfw.SwapBuffers()
}

// Clear clears the window.
func Clear() {
	gl.Clear(gl.COLOR_BUFFER_BIT)
}

// An Image is a drawable image.
type Image struct {
	tex gl.Texture

	// Width and Height are the size of the image.
	// They may be change to modify it's size.
	Width, Height int
}

// MakeImage makes an image from an image.NRGBA.
func MakeImage(i *image.NRGBA) (img Image) {
	img.Width, img.Height = i.Bounds().Dx(), i.Bounds().Dy()

	img.tex = gl.GenTexture()
	img.tex.Bind(gl.TEXTURE_2D)
	gl.TexParameterf(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR)
	gl.TexParameterf(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR)

	gl.TexImage2D(gl.TEXTURE_2D, 0, 4, img.Width, img.Height,
		0, gl.RGBA, gl.UNSIGNED_BYTE, i.Pix)
	return
}

// LoadPng loads a image from the given PNG file.
func LoadPng(file string) (Image, error) {
	img, err := loadPng(file)
	if err != nil {
		return Image{}, err
	}
	return MakeImage(img), nil
}

// loadPng loads an image from a .png file.
//
// The image must be a NRGBA image... whatever that means.
func loadPng(file string) (rgbaImg *image.NRGBA, err error) {
	in, err := os.Open(file)
	if err != nil {
		return
	}
	defer in.Close()

	img, err := png.Decode(in)
	if err != nil {
		return
	}

	rgbaImg, ok := img.(*image.NRGBA)
	if !ok {
		err = errors.New("texture must be an NRGBA image")
	}
	return
}

// Draw draws the given image to the open window.
func (img Image) Draw(x, y int) {
	img.tex.Bind(gl.TEXTURE_2D)
	gl.Begin(gl.QUADS)
	gl.TexCoord2i(0, 0)
	gl.Vertex3i(x, y, 0)
	gl.TexCoord2i(1, 0)
	gl.Vertex3i(x+img.Width, y, 0)
	gl.TexCoord2i(1, 1)
	gl.Vertex3i(x+img.Width, y+img.Height, 0)
	gl.TexCoord2i(0, 1)
	gl.Vertex3i(x, y+img.Height, 0)
	gl.End()
	img.tex.Unbind(gl.TEXTURE_2D)
}

// Release releases the resources that were allocated
// for this image.  The image is then rendered unusable.
func (img Image) Release() {
	img.tex.Delete()
}

// A Font describes the look of and draw text.
type Font struct {
	ctx  *freetype.Context
	fnt *truetype.Font
	sz float64
}

// LoadTtf returns a truetype font loaded from the given file.
func LoadTtf(file string, sz int, c color.Color) (font Font, err error) {
	bytes, err := ioutil.ReadFile(file)
	if err != nil {
		return
	}

	fnt, err := freetype.ParseFont(bytes)
	if err != nil {
		return
	}

	font.ctx = freetype.NewContext()
	font.ctx.SetDPI(72)
	font.ctx.SetFont(fnt)
	font.ctx.SetFontSize(float64(sz))
	font.ctx.SetSrc(image.NewUniform(c))
	font.fnt = fnt
	font.sz = float64(sz)
	return
}

// Render renders the text in the given font and returns an image
// of the formatted string.
func (font Font) Render(format string, vls ...interface{}) (img Image, err error) {
	str := fmt.Sprintf(format, vls...)
	width, height := font.textSize(str)

	rgba := image.NewNRGBA(image.Rect(0, 0, width, height))
	draw.Draw(rgba, rgba.Bounds(), image.Black, image.ZP, draw.Src)
	font.ctx.SetDst(rgba)
	font.ctx.SetClip(rgba.Bounds())

	pt := freetype.Pt(0, height)
	pt, err = font.ctx.DrawString(str, pt)
	if err != nil {
		return
	}

	img = MakeImage(rgba)
	return
}

func (f Font) textSize(s string) (int, int) {
	// scale converts truetype.FUnit to float64
	scale := f.sz / float64(f.fnt.FUnitsPerEm())

	width := 0
	prev, hasPrev := truetype.Index(0), false
	for _, rune := range s {
		index := f.fnt.Index(rune)
		if hasPrev {
			width += int(f.fnt.Kerning(f.fnt.FUnitsPerEm(), prev, index))
		}
		width += int(f.fnt.HMetric(f.fnt.FUnitsPerEm(), index).AdvanceWidth)
		prev, hasPrev = index, true
	}
	width = int(float64(width)*scale + 0.5)

	b := f.fnt.Bounds(f.fnt.FUnitsPerEm())
	height := int(float64(b.YMax-b.YMin)*scale + 0.5)
	return width, height
}

// Draw draws text at the given location using the given font,
// returning the size of the image that was just drawn.
func (font Font) Draw(x, y int, format string, vls ...interface{}) (w, h int, err error) {
	img, err := font.Render(format, vls...)
	if err != nil {
		return
	}
	defer img.Release()
	img.Draw(x, y)
	w, h = img.Width, img.Height
	return
}
