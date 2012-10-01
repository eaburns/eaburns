// The fixed package implements fixed-point operations with
// 24 bits for the whole part and 8 bits for the fractional part.
package fixed32

// A Fixed32 is a 32-bit fixed-point value with 24 bits for the
// whole part and 8 bits for the fractional part.
type Fixed32 int32

const (
	// shift is the number of bits by which the
	// whole part of the number is shifted.
	shift = 8

	// Zero is the zero value.
	Zero = Fixed32(0)

	// One is the one value.
	One = Fixed32(1 << shift)
)

// Make returns a Fixed32 from a pair of integers, the first
// represents the whole part and the second is the fracitonal
// part.
func Make(w, f int) Fixed32 {
	return Fixed32(w<<shift + f)
}

// Add returns the sum of two Fixed32 numbers.
func (a Fixed32) Add(b Fixed32) Fixed32 {
	return a + b
}

// Add returns the difference of two Fixed32 numbers.
func (a Fixed32) Sub(b Fixed32) Fixed32 {
	return a - b
}

// Mul returns the product of two Fixed32 numbers.
func (a Fixed32) Mul(b Fixed32) Fixed32 {
	return Fixed32((int64(a) * int64(b)) >> shift)
}

// Div returns the quotient of two Fixed32 numbers.
func (a Fixed32) Div(b Fixed32) Fixed32 {
	return Fixed32((int64(a) / int64(b)) << shift)
}

// Mod returns the remainder when dividing two Fixed32 numbers.
func (a Fixed32) Mod(b Fixed32) Fixed32 {
	return a % b
}

// Whole returns the whole portion of the Fixed32 number.
func (a Fixed32) Whole() int {
	return int(a >> shift)
}

// Frac returns the fractional portion of teh Fixed32 number.
func (a Fixed32) Frac() float32 {
	return float32(a&0xFF) / float32(1<<shift)
}
