// Hiding and fields
int main()
{
  int i;
  T t;

  i = t.field; // Make sure we use the correct, non-hidden field.
}

class T extends S {
  int field; // Hides S.field
  T(Object o) {
    o = super.field; // Accessed using super
  }
}

class S {
  Object field;
}
