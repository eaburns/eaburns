// Method invocation -- determine class to search
class T {
  int i;

  int method() {
    i = method();
    i = this.method();
  }
}

class S extends R {
  S s;

  S method() {
    s = method();
    s = super.method();
  }
}

class R {
  S method() {}
}

int main()
{
  S s;
  T t;
  int i;

  s = s.method();
  i = t.method();
  method();
  this.method();
  super.method();
}
