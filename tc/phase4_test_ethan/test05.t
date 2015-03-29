// Test variable initialization with method and constructor parameters

class S {
  S(int i) { 
    out i; 
  }
}

class T extends S {
  T(int i, int j) {
    super(i);
    out j;
  }

  int T(int i, int j) {
    out i;
    out j;
  }
}

int main()
{
  new T(1, 2).T(3, 4);

  return 0;
}
