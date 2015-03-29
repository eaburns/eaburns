// Casting conversion
int main()
{
  T t, tt[];
  S s;
  R r;
  int i;

  t = (T)t;     // Identity
  tt = (T[])tt; // Identity (arrays)
  s = (S)t;     // Widening
  t = (T)s;     // Narrowing
  t = (T)i;     // Error (Forbidden)
  // i = (int)t was tested, results in syntax error
}

class T extends S {}
class S {}
class R {}
