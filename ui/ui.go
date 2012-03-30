package ui

import (
	gl "github.com/chsc/gogl/gl21"
	"github.com/jteeuwen/glfw"
	"io"
	"errors"
	"image"
	"image/png"
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
	r, g, b := 0, 0, 0	// defaults
	a := 8	// 8-bit alpha channel
	depth, stencil := 0, 0	// no depth or stencil buffers
	mode := glfw.Windowed

	glfw.OpenWindowHint(glfw.WindowNoResize, 1)

	if err := glfw.OpenWindow(w, h, r, g, b, a, depth, stencil, mode); err != nil {
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
type Drawer interface{
	// Draw draws a Drawer at the given x, y window
	// coordinate.
	Draw(x, y int)
}

// An Image is a drawable image.
type Image struct{
	texId gl.Uint
	W, H int
}

// ReadImage creates a new image by reading a .png from
// the given io.Reader
func ReadImage(in io.Reader) (Image, error) {
	img, err := png.Decode(in)
	if err != nil {
		return Image{}, err
	}

	rgbaImg, ok := img.(*image.NRGBA)
	if !ok {
		return Image{}, errors.New("texture must be an NRGBA image")
	}
	w, h := rgbaImg.Bounds().Dx(), rgbaImg.Bounds().Dy()

	var texId gl.Uint
	gl.GenTextures(1, &texId)
	gl.BindTexture(gl.TEXTURE_2D, texId)
	gl.TexParameterf(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR)
	gl.TexParameterf(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR)
	gl.TexImage2D(gl.TEXTURE_2D, 0, 4, gl.Sizei(w), gl.Sizei(h),
		0, gl.RGBA, gl.UNSIGNED_BYTE, gl.Pointer(&rgbaImg.Pix[0]))

	return Image{texId: texId, W: w, H: h}, nil
}

// Draw draws the given image to the open window.
func (i Image) Draw(x, y int) {
        gl.BindTexture(gl.TEXTURE_2D, i.texId);
        gl.Begin(gl.QUADS);
        gl.TexCoord2i(gl.Int(0), gl.Int(0));
        gl.Vertex3i(gl.Int(x), gl.Int(y), gl.Int(0));
        gl.TexCoord2i(gl.Int(1), gl.Int(0));
        gl.Vertex3i(gl.Int(x+i.W), gl.Int(y), gl.Int(0));
        gl.TexCoord2i(gl.Int(1), gl.Int(1));
        gl.Vertex3i(gl.Int(x+i.W), gl.Int(y+i.H), gl.Int(0));
        gl.TexCoord2i(gl.Int(0), gl.Int(1));
        gl.Vertex3i(gl.Int(x), gl.Int(y+i.H), gl.Int(0));
        gl.End();
}
