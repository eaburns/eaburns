// Tests that a variable of a reference type can hold either a null 
// reference, or a reference to any object that is assignment compatable.

int main()
{
  T t;
  S s;
  R r;
  int i;

  s = null;
  s = t;
  t = s; // Error
  t = r; // Error
  s = r; // Error
  t = i; // Error
}

class T extends S {}

class S {}

class R {}
