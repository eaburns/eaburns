// Test `this'

class S {
  int method() {
    this.method2();
  }

  int method2() {
    out -1;
  }
}

class T extends S {
  int method2() {
    out 1;
  }
}

int main() 
{
  S s;

  s = new T();

  s.method();
  
  return 0;
}
