// Finding applicable methods
class T extends S {
  int meth() {}
  int meth(int i) {}
  int meth(int i, int j) {}
  T methT() {}
  T methT(S s) {}
  T methT(T t) {}
}

class S {}

int main()
{
  T t;

  t.meth();
  t.meth(1);
  t.meth(1, 2);
  t.methT();
  t.methT(new S());
  t.methT(t); // Error
}
