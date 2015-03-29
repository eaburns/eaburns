// Cast expressions
class S {}
class T extends S {}

int main()
{
  S s; T t; int i;

  s = (S)t;
  s = (S)s;
  t = (T)s;
  t = (5 + 6) s; // Error
  t = (t)s; // Error
  t = (T)i; // Error
  t = (T)5 + 7; // Error
}
