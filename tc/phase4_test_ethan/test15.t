// Tests return from constructor or destructor

class T {
  T() {
    out 1;
    return;
    out -1;
  }

  ~T() {
    out 2;
    return;
    out -1;
  }
}

int main()
{
  T t;
  
  t = new T();
  delete t;

  return 0;
}
