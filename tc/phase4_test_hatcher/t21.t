class A {
  int a;
  A b;
  int c;
}

int main()
{
  A a;

  a = new A();
  a.c = 7;

  a.b = new A();

  a.b.a = 7;
  a.b.c = 7;

  out(a.b.a + a.b.c + a.c);
}
