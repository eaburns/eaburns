// Explicit constructor invocation
int main() {}

class T extends S {
  S field;

  T(S s) {
    super(field); // Can not access a field in this instance durring 
                  // explicit constructor invocation.
  }
}

class S {
  S(S s) {}
}
