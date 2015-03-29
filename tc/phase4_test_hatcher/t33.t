

class A {
  int a;
  int b;
  int c;
  ~A() {
     out(a+b+c);
  }
}

class B extends A {
  ~B() {
    out(33);
  }
}

int main()
{
  B b;

  b = new B();
  delete b;

  A a;
  a = new A();
  a.a = 11;
  a.b = 11;
  a.c = 11;
  delete a;

}
