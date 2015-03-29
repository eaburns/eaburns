/*
 * type.c defines functions to make type data structures.
 * Ethan Burns -- Created Wed Feb 22 20:17:24 2006
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "type.h"
#include "mtrack.h"

/*
 * The allocated memory for the class table. 
 */
static struct memassoc *memory = NULL;

/*
 * Since the only thing needed for an INT_TYPE is to know the tag, there
 * is only one actual structure and the same pointer is returned all the
 * time. 
 */
static Type int_type = { INT_TYPE };
static Type null_type = { NULL_TYPE };
static Type error_type = { ERROR_TYPE };

void
initTypes()
{
  memory = make_memassoc();
}

void
freeTypes()
{
  t_free_all(memory);
}

Type *
makeErrorType()
{
  return &error_type;
}

Type *
makeIntType()
{
  return &int_type;
}

Type *
makeNullType()
{
  return &null_type;
}

ArrayType *
makeArrayType(Type * next)
{
  ArrayType *t = t_malloc(memory, sizeof(ArrayType));
  t->super.tag = ARRAY_TYPE;
  t->next = next;

  return t;
}

ClassType *
makeClassType(char *name)
{
  ClassType *t = t_malloc(memory, sizeof(ClassType));
  t->super.tag = CLASS_TYPE;
  t->name = name;
  t->fields = NULL;
  t->numFields = 0;
  t->methods = NULL;
  t->cons = NULL;
  t->parent = NULL;
  t->next = NULL;

  return t;
}

FieldType *
makeFieldType(char *name, Type * type)
{
  FieldType *t = t_malloc(memory, sizeof(FieldType));
  t->super.tag = FIELD_TYPE;
  t->name = name;
  t->type = type;
  t->next = NULL;

  return t;
}

ParamType *
makeParamType(char *name, Type * type, int num)
{
  ParamType *t = t_malloc(memory, sizeof(ParamType));
  t->super.tag = PARAMETER_TYPE;
  t->name = name;
  t->type = type;
  t->num = num;
  t->next = NULL;

  return t;
}

MethodType *
makeMethodType(char *name, Type * ret)
{
  MethodType *t = t_malloc(memory, sizeof(MethodType));

  t->super.tag = METHOD_TYPE;
  t->name = name;
  t->type = ret;
  t->params = NULL;
  t->numParams = 0;
  t->class = NULL;
  t->next = NULL;
  t->def = 0;
  t->invokedConst = NULL;

  return t;
}

MethodType *
makeConstructorType(char *name)
{
  MethodType *t = t_malloc(memory, sizeof(MethodType));

  t->super.tag = CONSTRUCTOR_TYPE;
  t->name = name;
  t->type = NULL;
  t->params = NULL;
  t->numParams = 0;
  t->class = NULL;
  t->next = NULL;
  t->def = 0;
  t->invokedConst = NULL;

  return t;
}

Type *
appendArray(Type * t, ArrayType * a)
{
  ArrayType *p;

  if (a == NULL)
    return t;

  for (p = a; p->next != NULL && p->super.tag == ARRAY_TYPE;
       p = (ArrayType *) p->next);

  assert(p->next == NULL);
  p->next = t;

  return &a->super;
}

Type *
copyArrays(Type * t)
{
  if (t == NULL)
    return NULL;

  switch (t->tag) {
  case ARRAY_TYPE:
    return &makeArrayType(copyArrays(((ArrayType *) t)->next))->super;
  default:
    return t;
  }
}

int
equal_types(Type * a, Type * b)
{
  assert(a != NULL);
  assert(b != NULL);

  switch (a->tag) {
  case INT_TYPE:
    if (b->tag == INT_TYPE)
      return 1;
    else
      return 0;

  case CLASS_TYPE:
    /*
     * Must point to the same ClassType. 
     */
    if (a == b)
      return 1;
    else
      return 0;

  case ARRAY_TYPE:
    if (b->tag == ARRAY_TYPE)
      return equal_types(((ArrayType *) a)->next, ((ArrayType *) b)->next);
    else
      return 0;

    /*
     * No other types should get passed in here. 
     */
  default:
    return 0;
  }

  /*
   * Unreachable. 
   */
  assert(0);
  return 0;
}

int
same_sigs(MethodType * a, MethodType * b)
{
  ParamType *p,
    *q;

  /*
   * Check names first. 
   */
  if (strcmp(a->name, b->name) != 0) {
    return 0;
  }

  /*
   * Next: scan the parameters. 
   */
  p = a->params;
  q = b->params;
  while (p != NULL && q != NULL) {

    if (equal_types(p->type, q->type) != 1)
      return 0;
    p = p->next;
    q = q->next;
  }

  /*
   * If one list has more parameters, while the other does not, return
   * 0. 
   */
  if (q != p)
    return 0;

  /*
   * All tests passed, the signatures are the same. 
   */
  return 1;
}

Type *
arrayBaseType(ArrayType * t)
{
  Type *p;

  for (p = &t->super; p->tag == ARRAY_TYPE; p = ((ArrayType *) p)->next);

  assert(p != NULL);
  assert(p->tag != ARRAY_TYPE);

  return p;
}

unsigned int
arrayDims(ArrayType * t)
{
  unsigned int count = 0;
  Type *p;

  for (p = &t->super; p->tag == ARRAY_TYPE; p = ((ArrayType *) p)->next)
    count++;

  return count;
}

int
isReference(Type * t)
{
  return t->tag == CLASS_TYPE || t->tag == ARRAY_TYPE;
}

void
printType(Type * t)
{
  Type *p;

  if (t == NULL)
    return;

  switch (t->tag) {
  case INT_TYPE:
    printf("int");
    break;
  case ARRAY_TYPE:

    for (p = t; p != NULL && p->tag == ARRAY_TYPE;
         p = ((ArrayType *) p)->next);
    if (p != NULL)
      printType(p);

    for (p = t; p != NULL && p->tag == ARRAY_TYPE;
         p = ((ArrayType *) p)->next) {
      printf("[]");
    }

    break;
  case CLASS_TYPE:
    printf("%s", ((ClassType *) t)->name);
    break;
  case FIELD_TYPE:
    printf("Field type");
    break;
  case METHOD_TYPE:
    printf("Method type");
    break;
  case CONSTRUCTOR_TYPE:
    printf("Constructor type");
    break;
  case PARAMETER_TYPE:
    printf("Parameter type");
    break;
  case NULL_TYPE:
    printf("null type");
    break;
  case ERROR_TYPE:
    printf("error type");
    break;
  }
}

char *
getTypeName(Type * t)
{
  Type *p;
	unsigned int len;
	char *base_type;
	char *retval;

  if (t == NULL) {
		retval = malloc(sizeof(char));	
		retval[0] = '\0';
		return retval;
	}

  switch (t->tag) {
  case INT_TYPE:
		retval = malloc(sizeof(char)*(strlen("int")+1));
		strcpy(retval, "int");
		return retval;

  case ARRAY_TYPE:

		/* Find the size of the string. */
    for (p = t; p != NULL && p->tag == ARRAY_TYPE;
         p = ((ArrayType *) p)->next);
      base_type = getTypeName(p);

		len = sizeof(base_type) + 1;

    for (p = t; p != NULL && p->tag == ARRAY_TYPE;
         p = ((ArrayType *) p)->next) {
			len += 2;
    }

		/* build the string. */
		retval = malloc(sizeof(char)*len);	
		strncpy(retval, base_type, len);
		free(base_type);

    for (p = t; p != NULL && p->tag == ARRAY_TYPE;
         p = ((ArrayType *) p)->next)
			strncat(retval, "[]", len);
		
		return retval;

  case CLASS_TYPE:
		len = strlen(((ClassType *) t)->name) + 1;
		retval = malloc(sizeof(char)*len);
		strcpy(retval,((ClassType *) t)->name);
		return retval;

	default: break;
  }

	retval = malloc(sizeof(char) * (strlen("error") + 1));
	strcpy(retval, "error");

	return retval;
}
