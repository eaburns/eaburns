#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "symtab.h"
#include "type.h"
#include "class.h"
#include "list.h"

static List *head = NULL;

void
addSymbol(Type * type, char *name)
{
  Symbol *p;

  p = malloc(sizeof(Symbol));
  if (p == NULL) {
    fprintf(stderr, "addSymbol: malloc out of memory.\n");
    exit(1);
  }

  p->name = name;
  p->type = type;

  head = list_add(head, p);

#if 0
  printf("Added symbol [");
  printType(type);
  printf(" %s]\n", name);
#endif
}

Type *
lookupType(const char *name)
{
  List *p;
  Symbol *sym;

  p = head;
  while (p != NULL) {
    sym = p->data;
    if (strcmp(sym->name, name) == 0) {
      return sym->type;
    }
    p = p->next;
  }

  return NULL;
}
