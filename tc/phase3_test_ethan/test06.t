// Forbidden conversions, should be all errors
int main()
{
  int i;
  T t, tArray[];
  S s;
  R r, rArray[];
  int array[][];

  i = t;    // Error
  i = null; // Error
  t = r; r = t; // Error, Error
  array = t; // Error
  t = array; // Error
  tArray = rArray; rArray = tArray; // Error, Error
}

class T extends S {}
class S {}
class R {}
