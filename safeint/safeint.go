// The safeint package provides arithmetic operations on integers
// that are safe with respect to overflow and underflow.

package safeint

const (
	// MaxInt is the maximum representable value in an integer.
	MaxInt = int(^uint(0) >> 1)

	// MinInt is the minimum representable value in an integer.
	MinInt = -MaxInt - 1
)

// Add returns the sum of two integers.  The second return
// value is +1 for overflow, -1 for underflow and 0 otherwise.
func Add(a, b int) (int, int) {
	carry := 0
	if a > 0 && b > 0 && MaxInt - b < a {
		carry = 1
	} else if a < 0 && b < 0 && MinInt - b > a {
		carry = -1
	}
	return a + b, carry
}

// MustAdd returns the sum of the two integers and panics
// if there is overflow or underflow.
func MustAdd(a, b int) int {
	return must(Add, a, b)
}

// Sub returns the difference of two integers.  The second return
// value is +1 for overflow, -1 for underflow and 0 otherwise.
func Sub(a, b int) (int, int) {
	carry := 0
	if a > 0 && b < 0 && MaxInt + b < a {
		carry = 1
	} else if a < 0 && b > 0 && MinInt + b > a {
		carry = -1
	}
	return a - b, carry
}

// MustSub returns the difference of the two integers and panics
// if there is overflow or underflow.
func MustSub(a, b int) int {
	return must(Sub, a, b)
}

// Mul returns the product of two integers.  The second return
// value is +1 for overflow, -1 for underflow and 0 otherwise.
func Mul(a, b int) (int, int) {
	carry := 0
	switch {
	case a > 0 && b > 0 && MaxInt/b < a:
		carry = 1
	case a < 0 && b > 0 && MinInt/a < b:
		carry = -1
	case a > 0 && b < 0 && MinInt/b < a:
		carry = -1
	case a < 0 && b < 0:
		if a == MinInt || b == MinInt || -(MinInt/b) > a {
			carry = 1
		}
	}
	return a*b, carry
}

// MustMul returns the product of the two integers and panics
// if there is overflow or underflow.
func MustMul(a, b int) int {
	return must(Mul, a, b)
}

// Div returns the quotient of two integers.  The second return
// value is +1 for overflow, -1 for underflow and 0 otherwise.
func Div(a, b int) (int, int) {
	carry := 0
	if b == 0 || (a == MinInt && b == -1) {
		carry = 1
	}
	return a/b, carry
}

// MustDiv returns the quotient of the two integers and panics
// if there is overflow or underflow.
func MustDiv(a, b int) int {
	return must(Div, a, b)
}

// must returns the result of the operation on the two values but
// panics if they overflow or underflow.
func must(oper func(int, int) (int, int), a, b int) int {
	switch r, carry := oper(a, b); {
	case carry == 0:
		return r
	case carry < 0:
		panic("Underflow");
	}
	panic("Overflow");
}