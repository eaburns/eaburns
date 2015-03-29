class A {
  int i;
  B b;
}

class B {
  int j;
  A a;
}

int main() {
  A a;
  B b;
  a.b.j = b.a.i;
}
