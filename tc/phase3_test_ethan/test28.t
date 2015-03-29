// Methods and overriding
int main() {}

class T extends S {
  int badmethod() {} // Error (return type change)
  int badmethod2() {} // Error (return type change)
  int method() {}
}

// Sub class declared first.
class S extends R {
  S badmethod() {}
  int method() {}
}

class R {
  int method() {}
  R badmethod2() {}
}

// Super class declared first.
class A {
  A badmethod3() {}
}

class B extends A {
  int badmethod3() {} // Error (return type change)
}
