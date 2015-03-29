// Field access
int main()
{
  i.asfd = 5; // Error
  new T().nothing = 5; // Error
  new T().field = 5;
}

class T extends S {
  int field;

  S s;
  int i;

  T() {
    s = super.field;
    i = this.field;
  }
}

class S {
  S field;
}
