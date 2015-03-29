// Equality operator
int main()
{
  Object o;
  int i;

  i = 1 == 1;
  i = null == null;
  i = o == o;
  i = null == o;
  i = o == null;
  i = i == null; // Error
  i = null == 1; // Error
  i = o == 1; // Error
  i = 1 == o; // Error
  o = 1 == 2; // Error
  o = null == o; // Error
}
