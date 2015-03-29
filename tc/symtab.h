/*
 * symtab.h declares functions for manipulating the main variable symbol table.
 * Ethan Burns -- Created Wed Mar  1 22:03:24 2006
 */
#ifndef _SYMTAB_H_
#define _SYMTAB_H_

#include "type.h"

typedef struct {
  char *name;
  Type *type;
} Symbol;

/*
 * Adds a symbol to the table. 
 */
void addSymbol(Type * type, char *name);

/*
 * Gets the type of a symbol given its name. 
 */
Type *lookupType(const char *name);

#endif                          /* !_SYMTAB_H_ */
