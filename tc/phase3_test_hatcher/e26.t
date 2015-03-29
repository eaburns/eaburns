class A {}
class B extends A{}
class C extends B{}
class D {}
class E {
  int f(int i1, B b, int i2, int i3) {}
}

int main()
{
  A a;
  B b;
  C c;
  D d;
  E e;
  int i;

  i = e.f(0,a,1,2);
  i = e.f(0,b,1,2);
  i = e.f(0,c,1,2);
  i = e.f(0,d,1,2);

  i = e.f(0,1,2,3);

  i = e.f(0,b,2,a);
}
