// Variable initialization
// NOTE: references tested to have a value of null, they
//       gave null pointer exceptions.

class T {
  int i;
}

int main()
{
  int i;
  T t;

  t = new T();

  out i;
  out t.i;

  // Make sure the field is re-initialized to zero
  t.i = 6;
  delete t;
  t = new T();
  out t.i;

  return 0;
}
