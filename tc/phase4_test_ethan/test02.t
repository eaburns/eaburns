// Test Object as the super class of all other classes.

class S {}

class T extends S {}

int main()
{
  Object o;

  o = new S();
  delete o;

  o = new T();
  delete o;

  return 0;
}
