
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

int main()
{
  A a;
  int q1;
  int q2;
  int q3;

  a = new A();
  q1 = 10;
  q2 = 2;
  q3 = 11;

  a.f1(q1, q2, q3 + 4);
  out(a.a + a.b + a.c);
}
