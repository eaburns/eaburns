// Multiple fields declared in one declaration
int main()
{
  int i;
  T t;

  i = t.i;
  i = t.ii;
}

class T {
  int i, ii;
}
