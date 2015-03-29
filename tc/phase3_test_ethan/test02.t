// Tests 2.3.2, The class Object is a superclass of all classes, and a 
// variable of type Object can hold a reference to any class instance or
// array.
int main()
{
  Object o;
  int i[][];
  T t;

  o = i;
  o = t;
}

class T {}
