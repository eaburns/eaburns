class B extends A
{
  int meth()
  {
    super.meth();
    out(25);
  }
}

class A
{
  int meth()
  {
    out(1);
  }
}

class C extends B
{
}

int main()
{
  A a;
  B b;
  C c;

  a = new A();
  b = new B();
  c = new C();
  b.meth();
  c.meth();
}

