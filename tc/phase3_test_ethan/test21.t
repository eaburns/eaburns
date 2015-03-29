// Can't have two fields with the same name
int main() {}

class T {
  int field, field; // Error
  int field; // Error
}
