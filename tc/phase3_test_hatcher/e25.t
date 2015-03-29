class A {}
class B extends A{}
class C extends B{}
class D {}
class E {
  int f(B b) {}
}

int main()
{
  A a;
  B b;
  C c;
  D d;
  E e;
  int i;

  i = e.f(a);
  i = e.f(b);
  i = e.f(c);
  i = e.f(d);
}
