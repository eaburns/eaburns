// Test narrowing reference conversion

class U extends T {}
class T extends S {}
class S {}

int main()
{
  int i;
  S s;
  T t;
  U u;

  // identity converstion
  s = (S) new S();

  // widening conversion

  s = (S) new T();

  // narrowing conversions
  s = new U();

  t = (T) s;
  u = (U) s;

  s = new S();
  u = (U) s; // Error

  return 0;
}
