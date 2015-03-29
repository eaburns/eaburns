class A {
}

class B extends A {
}

class C extends B {
}

class D extends C {
}

int main()
{
  A a;
  B b;
  C c;
  D d;

  d = new D();

  b = (B) d;
  c = (C) b;
  a = (A) d;
  c = (C) a;
  out 35;
}
