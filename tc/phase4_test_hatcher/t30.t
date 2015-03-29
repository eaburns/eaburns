
class A {
  int a;
  A b;
  int c;

  int f1(int x, A y, int z) {
    a = x;
    b = y;
    c = z;
  }

  A f2() {
    return b;
  }
  
  A f3() {
    return this;
  }
  
}

int main()
{
  A a;
  a = new A();

  a.f1(12, a, 6);
  out(a.a + a.f2().a + a.c);
  out(a.a + a.f3().a + a.c);
}
