// Methods, fields, types can have the same name
int main() 
{
  int i;
  T t;

  i = t.S;
  i = t.S();
}

class T {
  int S;
  int S() {}
}

class S {}
