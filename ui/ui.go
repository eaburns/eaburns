package ui

import (
	"errors"
	gl "github.com/chsc/gogl/gl21"
	"github.com/jteeuwen/glfw"
	"image"
	"image/png"
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

	if err := gl.Init(); err != nil {
		return err
	}

	gl.Enable(gl.TEXTURE_2D)
	gl.Enable(gl.BLEND)
	gl.BlendFunc(gl.SRC_ALPHA, gl.ONE)
	gl.ClearColor(0.0, 0.0, 0.0, 0.0)
	gl.MatrixMode(gl.PROJECTION)
	gl.LoadIdentity()
	gl.Ortho(0, gl.Double(w), 0, gl.Double(-h), -1, 1)
	gl.MatrixMode(gl.MODELVIEW)
	gl.LoadIdentity()
	gl.Translated(gl.Double(0), gl.Double(-h), gl.Double(0))
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

// A Drawer is something that can draw itself.
type Drawer interface {
	// Draw draws a Drawer at the given x, y window
	// coordinate.
	Draw(x, y int)
}

// An Image is a drawable image.
type Image struct {
	texId gl.Uint

	// Width and Height are the size of the image.
	// They may be change to modify it's size.
	Width, Height int
}

// LoadPng loads a image from the given PNG file.
func LoadPng(file string) (img Image, err error) {
	i, err := loadPng(file)
	if err != nil {
		return
	}

	img.Width, img.Height = i.Bounds().Dx(), i.Bounds().Dy()

	gl.GenTextures(1, &img.texId)
	gl.BindTexture(gl.TEXTURE_2D, img.texId)
	gl.TexParameterf(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR)
	gl.TexParameterf(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR)

	gl.TexImage2D(gl.TEXTURE_2D, 0, 4, gl.Sizei(img.Width),
		gl.Sizei(img.Height), 0, gl.RGBA, gl.UNSIGNED_BYTE,
		gl.Pointer(&i.Pix[0]))
	return
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
func (i Image) Draw(x, y int) {
	gl.BindTexture(gl.TEXTURE_2D, i.texId)
	gl.Begin(gl.QUADS)
	gl.TexCoord2i(gl.Int(0), gl.Int(0))
	gl.Vertex3i(gl.Int(x), gl.Int(y), gl.Int(0))
	gl.TexCoord2i(gl.Int(1), gl.Int(0))
	gl.Vertex3i(gl.Int(x+i.Width), gl.Int(y), gl.Int(0))
	gl.TexCoord2i(gl.Int(1), gl.Int(1))
	gl.Vertex3i(gl.Int(x+i.Width), gl.Int(y+i.Height), gl.Int(0))
	gl.TexCoord2i(gl.Int(0), gl.Int(1))
	gl.Vertex3i(gl.Int(x), gl.Int(y+i.Height), gl.Int(0))
	gl.End()
}
