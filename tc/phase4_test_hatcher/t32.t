
class A {
  int a;
  int b;
  int c;
  A(int x) {
    a = x;
    b = x;
    c = x;
  }
}

class B extends A {
  B() {
    super(10);
  }
}

int main()
{
  B b;

  b = new B();

  out(b.a+b.b+b.c+2);

  A a;
  a = new A(11);
  out(a.a+a.b+a.c-1);

}
