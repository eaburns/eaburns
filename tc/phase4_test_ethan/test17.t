// Test dynamic method dispatching

class S {
  int method() {
    out 2;
  }
}

class T extends S {
  int method() {
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
