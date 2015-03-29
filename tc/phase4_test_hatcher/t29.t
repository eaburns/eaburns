
class A {
  int a;
  A b;
  int c;

  int f1(int x, A y, int z) {
    a = x;
    b = y;
    c = z;
  }
  
}

int main()
{
  A a;
  a = new A();

  a.f1(12, a, 5);
  out(a.a + a.b.a + a.c);
}
