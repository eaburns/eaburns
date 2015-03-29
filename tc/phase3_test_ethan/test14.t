// Test type names
// There should be a bunch of errors for ``class name expected''
int main()
{
  Q q; // Error
}

class R extends Q { // Error, no class Q
  Object o;
  Q q;              // Error
  Q meth() {}       // Error
  int meth(Q q) {}  // Error
  R(Q q) {}         // Error
  R() {
    o = new Q();    // Error
    // Array access also
  }
}
