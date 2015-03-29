
class A {
  int a;
  A b;
  int c;

  int f1(int x, int z) {
    a = x;
    b = this;
    c = z;
  }

}

int main()
{
  A a;
  a = new A();

  a.f1(12, 4);
  out(a.a + a.b.a + a.c);
}
