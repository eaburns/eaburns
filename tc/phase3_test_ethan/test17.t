// Meaning of method names
int main()
{
  T t;
  int i;

  i = t.method();
}

class T extends S {
  int i;
  S s;

  int method() {}


  T() {
    i = method(); // Simple method name
    i = this.method(); // Qualified method name
    s = super.methodS();
  }
}

class S {
  S methodS() {}
}
