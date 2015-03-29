// Test destructor invocations
class S {
  ~S() { out 2; }
}

class T extends S {
  ~T() { out 1; }
}

int main()
{
  T t;

  t = new T();
  delete t;

  return 0;
}
