// Tests assignment conversion allowing the use of identity and widening
// reference, and not narrowing reference.
int main()
{
  T t;
  S s;
  int array[];

  s = s;
  t = t;
  s = t;
  t = s; // Error
  s = null;
  t = null;
  array = null;
}

class T extends S {}
class S {}
