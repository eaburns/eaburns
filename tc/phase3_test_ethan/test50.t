// Comparison operators < and >
int main()
{
  Object o;
  int i;

  i = 1 < 2;
  i = 1 > 2;
  i = o < 2; // Error
  i = 1 < o; // Error
  i = o > 2; // Error
  i = 1 > o; // Error
  o = 1 < 2; // Error
  o = 1 > 2; // Error
}
