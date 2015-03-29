// Constructor body
class S {
  S() {}
  S(int i) {}
}

class T extends S {

  T() {
    this(5); // A method invocation using this
  } 

  T(int i) {
    super(i); // A method invocation using super
  }

  T(int i, int j) {
    this(i, j, 1);
  }

  T(int i, int j, int k) {
    this(i, j, 1, 2);
  }

  T(int i, int j, int k, int l) {
    this(i, j); // Error (Circular)
  }
}

int main() {}
