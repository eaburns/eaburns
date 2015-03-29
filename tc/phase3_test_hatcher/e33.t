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
  a = c;
  a = d;
  b = a;
  b = b;
  b = c;
  b = d;
  c = a;
  c = b;
  c = c;
  c = d;
  d = a;
  d = b;
  d = c;
  d = d;
  i = a;
  a = i;
}
