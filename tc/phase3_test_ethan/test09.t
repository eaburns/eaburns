// Method invocation conversion.
int main()
{
  R r;
  T t;
  S s;

  r.methT(t); // Identity
  r.methS(t); // Widening
  r.methT(s); // Error (Narrowing)
}

class T extends S {}
class S {}

class R {
  int methT(T t) {}
  int methS(S s) {}
}
