class A {
  int a;
}

class B extends A {
  A a;
  int f(A a) { return a.a + this.a.a + super.a; }
}
int main() { }
