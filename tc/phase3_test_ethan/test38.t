// Return statement
int main()
{
  return new T(); // Error
  return 5;
}

class T extends S {
  T() { 
    return;
    return 5; // Error
  }

  ~T() {
    return;
    return 213; // Error
  }

  S meth() {
    return new T();
    return; // Error
    return 2; // Error
  }
}

class S {}
