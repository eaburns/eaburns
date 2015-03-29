class A {
  int i;
}

class B extends A {
  int j;

  int f() {
    return this.j + super.i;
  }
}

int main() { }
