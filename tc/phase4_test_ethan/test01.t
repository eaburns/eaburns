// Test that multiple variables can modify a class's state
// if they contain the same reference. (2.3.1)
class T {
  int field;

  T() { field = 0; }
}

int main()
{
  T t0, t1;

  t0 = new T();
  t1 = t0;

  t0.field = 5;
  out t1.field;

  return 0;
}
