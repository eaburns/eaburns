// Test evaulation order in expressions

class T {
  int meth1() { out 1; return 1; }
  int meth2() { out 2; return 2; }
}

int main()
{
  T t;

  t = new T();

  out t.meth1() + t.meth2();

  return 0;
}
