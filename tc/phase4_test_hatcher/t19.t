class A {
  int a;
  int b;
  int c;
}

int main()
{
  A a;

  a = new A();

  out(a.a+19);

  a.a = 9;
  a.b = 2;
  a.c = 8;

  out(a.a+a.b+a.c);
}
