// Field and method with the same name...
int main()
{
  int i;
  T t;

  i = t.name;
  t = t.name();
}

class T {
  int name;
  T name() {}
}
