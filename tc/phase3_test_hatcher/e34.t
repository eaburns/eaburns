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

  a = (A) a;
  a = (A) b;
  a = (A) c;
  a = (A) d;
  b = (B) a;
  b = (B) b;
  b = (B) c;
  b = (B) d;
  c = (C) a;
  c = (C) b;
  c = (C) c;
  c = (C) d;
  d = (D) a;
  d = (D) b;
  d = (D) c;
  d = (D) d;
  a = (A) i;
}
