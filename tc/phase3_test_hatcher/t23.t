class A {}
class B extends A{}
class C extends B{}
class D {}

int main()
{
  A a;
  B b;
  C c;
  D d;
  int i;

  a = a;
  a = b;
  c = (C) a;
  i = 19;
}
