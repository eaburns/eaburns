// Tests the bounds integer literals.
int main() 
{
  int i;

  // Largest decimal literal
  i = 2147483647;

  // Too large.
  i = 2147483648; // Error

  // Smallest.
  i = -2147483648;

  // Too small.
  i = -2147483649; // Error

  // Null literal in a binary subtraction, still too large.
  i = 1-2147483648; // Error
}
