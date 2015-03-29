// Test returning from while or if-then-else blocks
class T {
  int whileTest() {
    while(1) {
      return 1;
      out -1;
    }
  }
}

int main()
{
  T t;

  t = new T();
  t.whileTest();

  if(1) {
    return 0;
  } else {}

  out -1;

  return 0;
}
