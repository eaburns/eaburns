// while statement
class T {
  int field;

  T() { field = 4; }

  int decField() { 
    out field;

    field = field - 2; 
    return field + 2; 
  }
}

int main()
{
  int i;
  T t;

  t = new T();

  i = 3;
  while(t.decField()) {
    out i;
    i = i - 2;
  }

  while(0) {
    out -1;
  }

  return 0;
}
