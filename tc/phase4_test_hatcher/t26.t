class A {
  int a;
  int b;
  int c;

  int f1(int a) {
    out(this.a + 26);
    this.a = a;
  }
}

int main()
{
  A a;

  a = new A();
  a.f1(26);
  out(a.a);
}
