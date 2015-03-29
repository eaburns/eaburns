// Qualified expression names
int main()
{
  T t;
  S s;
  int i;

  i = i.nothing; // Error
  i = t.nothing; // Error
  i = s.field;
}

class T extends S {
  T field;

  T() {
    i = this.field; // Error
    i = super.field;
  }
}

class S {
  int field;
  int i;

  S() {
    i = this.field;
  }
}
