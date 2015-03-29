class A {
}

class B extends A {
}

int main()
{
  A a;
  B b;

  b = new B();
  a = (A) b;
  b = (B) a;
  out(39);

  a = new A();
  b = (B) a;
}
