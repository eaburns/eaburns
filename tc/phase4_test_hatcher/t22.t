class A {
  int a;
  int b;
  int c;
  A(int i) {
    a = i;
    b = a + 2;
    c = b + 3;
  }
}

int main()
{
  A a;

  a = new A(5);

  out(a.a + a.b + a.c);
}
