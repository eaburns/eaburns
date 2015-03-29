// Shadowing
int main() 
{
}

class S {}

class T {
  int i;

  S field;

  T(int field) {
    // If shadowing doesn't work then we would be trying to do an assignment
    // from type S to type int.  However, since shadowing DOES work the 
    // assignment IS okay 'cause we use the parameter not the field.
    i = field;
  }  
}
