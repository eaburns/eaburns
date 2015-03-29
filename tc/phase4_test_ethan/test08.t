// ``Drop off'' return values

class T {
  int integer() {}
  T reference() {}
}

int main()
{
  T t;

  out t.integer();
  out t.reference().integer(); // RTS error

  return 0;
}
