// Test expression names
int main()
{
  int var;

  // Appears with in a main block declaration
  var = var;
}

class T {
  T t;
  int i;
  T var;

  T(int var) {
    // With in a class, with in a visable parameter declaration
    i = var;
    t = var; // Error
  }

  T() {
    // With in a class, with in a visable field declaration
    t = var;
    i = var; // Error
  }
}
