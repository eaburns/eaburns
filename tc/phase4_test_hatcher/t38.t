class A {
  int a() { out(38);}
}

int main()
{
  A a;
  A a2;

  a = new A();

  if (!(a == null)) a.a(); else a = null;

  a2 = null;

  if (!(a == a2)) a2.a(); else a2 = null;
}
