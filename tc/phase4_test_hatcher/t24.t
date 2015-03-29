
class A {
  int a;
  int b;
  int c;
  A() {
   a = 1;
   b = 2;
   c = 3;
  }
  int sum() {
    return a + b + c;
  }
}

class B extends A {
  int b;
  int d;
  B() {
   b = 4;
   d = 5;
  }
  int sum() {
    return this.a + super.b + this.c;
  }
}

int main()
{
  A a;
  B b;

  a = new B();

  out(a.sum()+18);

  b = (B) a;

  out(b.sum()+18);
}
