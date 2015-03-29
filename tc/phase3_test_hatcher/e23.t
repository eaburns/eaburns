class A {
  A(int i) {}
}

class B {
  B(A a) {}
}

int main()
{
  A a;
  B b;

  a = new A(a);
  b = new B(19);
}
