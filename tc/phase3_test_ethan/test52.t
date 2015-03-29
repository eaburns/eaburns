// Assignment
class T {}
int main()
{
  T t;
  Object o;
  int i;

  i = 1; // Identity
  o = o; // Identity
  o = t; // Widening
  T = i; // Error
  i = o; // Error
  o = i; // Error
  t = o; // Error (Narrowing)
}
