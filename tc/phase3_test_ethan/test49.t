// Addative operators
int main()
{
  int i;
  Object o;

  i = 1 + 1;
  i = 1 - 1;
  i = 1 + o; // Error
  i = o + 1; // Error
  i = 1 - o; // Error
  i = o - 1; // Error
  o = i + 1; // Error
  o = i - 1; // Error
}
