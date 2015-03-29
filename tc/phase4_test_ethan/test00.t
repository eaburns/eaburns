// Test bounds of a integer constants
int main()
{
  int i;

  i = -2147483649; // Error
  i = -2147483648;
  i = 2147483647;
  i = 2147483648; // Error
  
  return 0;
}
