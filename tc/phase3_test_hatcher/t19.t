class A {
  int f1() {}
}

class B extends A {
  int f2() {}

  int f3() {
    return this.f2() + super.f1();
    return f2() + f1();
  }
}

int main()
{
  B b;
  int i;

  i = b.f2() + b.f1();
}
