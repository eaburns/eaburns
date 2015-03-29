#include <stdlib.h>
#include <stdio.h>
#include "literal.h"

Literal *
makeLiteral(char *str, unsigned int line)
{
  Literal *t = malloc(sizeof(Literal));
  if (t == NULL) {
    fprintf(stderr, "makeInt: malloc failed.\n");
    exit(1);
  }

  t->str = str;
  t->line = line;

  return t;
}
