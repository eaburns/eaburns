// Reference equality (2.3.3)
int main()
{
  T t, tt;
  S s, ss[][];
  int i[][], ii[][];

  t = tt;
  i = ii;
  t = s;  // Error
  s = i;  // Error
  ss = i; // Error
  ss = s; // Error
}

class T {}
class S {}
