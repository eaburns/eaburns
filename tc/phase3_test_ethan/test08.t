// More specific test on assignment conversion following all of the paths of
// the rules given in (3.2)
int main()
{
  Object o, oo[];
  R r, rr[];
  S s, ss[];
  T t, tt[];
  int i, ii[];

  // If Source is a class
  t = t; // Same class
  s = t; // Sub class
  r = t; // Error
  t = s; // Error
  i = t; // Error
  tt = o; // Error
  tt = t; // Error

  // If Source is an array type
  o = tt;
  t = tt; // Error

  // Target is an array and source is an array
  ii = ii; // Both ints.
  tt = tt; // Both refs, assignable
  ss = tt; // Both refs, assignable
  rr = tt; // Error
  tt = rr; // Error
}

class R {}
class S {}
class T extends S {}
