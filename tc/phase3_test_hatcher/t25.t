class A {
  int i;
  B b;
}

class B {
  C c;
}

class C {
  int j;
  int func(int i) { }
}

int main()
{
  A a;

  a.i = 19;

  a.b.c.func(a.b.c.j);

  a.b.c.func(a.b.c.func(a.b.c.func(a.b.c.j+1)));
}
