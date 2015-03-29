// Test method invocation using super
class S {
  int method() {
    out 1;
  }
}

class T extends S {
  int method() {
    super.method();
    out 2;
  }
}

int main()
{
  new T().method();

  return 0;
}
