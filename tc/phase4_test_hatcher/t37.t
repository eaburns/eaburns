class A {
  int a() { out(37);}
}

int main()
{
  A a;

  a = new A();

  a.a();

  a = null;

  a.a();
}
