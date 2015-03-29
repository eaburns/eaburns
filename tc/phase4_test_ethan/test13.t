// Test if-then-else statements
class T {
  int zero() { out 1; return 0; }
  int one() { out 3; return 1; }
}

int main()
{
  if(new T().zero()) { out 3; }
  else { out 2; }

  if(new T().one()) { out 4; }
  else { out 2; }

  return 0;
}
