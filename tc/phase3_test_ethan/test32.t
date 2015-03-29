// Destructor declarations
// Destructor with arguments was tested to be a syntax error
class T {
  ~T() {}
  ~T() {} // Error
  ~R() {} // Error
}

int main() {}
