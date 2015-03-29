// Constructors are not inherited
int main()
{
  T t;
  t = new T(5);
}

class S {
  S() {}
  S(int i) {}
}

class T extends S {}
