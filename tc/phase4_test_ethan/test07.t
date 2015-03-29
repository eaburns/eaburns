// Test reference types

class S {
  int field;

  S() { field = 2; }
}

class T extends S {
  int field;
  T() { field = 1; }

  int method() {
    out this.field;
    out field;
    out super.field;
  }

}

int main()
{
  T t;

  t = new T();
  out t.field;

  t.method();

  return 0;
}
