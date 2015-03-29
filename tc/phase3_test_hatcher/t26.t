class A {
  int f;
  int f(int f)
  {
    f = f;
    f = this.f;
    f = this.f(f);
    f = f(f);
  }
}

int main()
{
  int f;
  A a;

  a = new A();
  f = a.f;
  a.f = a.f(f);
}
