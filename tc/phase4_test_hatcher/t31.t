
class A {
  int a;
  int b;
  int c;

  int f1(int x, int y, int z) {
    a = x;
    b = y;
    c = z;
  }

}

class B {
  int meth(int i) {
    out i;
    return i;
  }
}

int main()
{
  A a;
  a = new A();
  B b;
  b = new B();

  a.f1(b.meth(7), b.meth(9), b.meth(11));
  out(a.a + a.b + a.c + 4);
}
