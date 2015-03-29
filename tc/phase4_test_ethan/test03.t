// Equals method of Object class.
class T {}

int main()
{
  Object o;
  T t;

  o = new Object();
  t = new T();

  out o.equals(t);
  out t.equals(o);
  delete o;

  o = t;
  out o.equals(t);
  out t.equals(o);
  delete t;

  return 0;
}
