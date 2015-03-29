// Test explicit constructor invocation

class R {
  R() { out -1; }
}

class S extends R {
  S(int i, int j) {}
}

class T extends S {
  T(U u) {
    super(u.one(), u.two());
  }
}

class U {
  int one() { out 1; }
  int two() { out 2; }
  int four() { out 4; }
  int five() { out 5; }
}

int main()
{
  T t;
  S s;
  U u;

  u = new U();
  t = new T(u);
  s = new S(u.four(), u.five());
  
  return 0;
}
