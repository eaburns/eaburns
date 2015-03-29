class A {}
class B extends A{}
class C extends B{}
class D {}
class E {
  E(B b) {}
}

int main()
{
  A a;
  B b;
  C c;
  D d;
  E e;

  e = new E(a);
  e = new E(b);
  e = new E(c);
  e = new E(d);
}
