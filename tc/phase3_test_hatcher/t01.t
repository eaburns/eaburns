class A {
  int i;
}

class B extends A {
  int j;
}

int main() {
  B b;
  int i;

  b = new B();

  i = b.j + b.i;
}

