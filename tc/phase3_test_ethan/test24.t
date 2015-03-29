// Method names
int main()
{
  int[][] i;
  T t;

  i = t.meth();
}

class T {
  int field;

  int T() {} // Same name as class
  int field() {} // Same name as field
  int meth()[][] {} // Array at end
}
