#include <stdlib.h>
#include <assert.h>
#include "conversion.h"
#include "type.h"
#include "class.h"

/*
 * Returns 1 if there is a widening reference conversion from src to dest.
 * If there is no widening reference covnersion, 0 is returend.
 */
int
widening_reference(Type * src, Type * dest)
{
  ArrayType *asrc,
  *adest;

  if (src->tag == NULL_TYPE && isReference(dest) == 1)
    return 1;

  if (src->tag == CLASS_TYPE) {
    if (dest->tag == CLASS_TYPE &&
        isSuper((ClassType *) dest, (ClassType *) src) == 1)
      return 1;
  } else if (src->tag == ARRAY_TYPE) {

    if (dest == &getClass(OBJECT_NAME)->super)
      return 1;

    if (dest->tag == ARRAY_TYPE) {
      asrc = (ArrayType *) src;
      adest = (ArrayType *) dest;
      return widening_reference(asrc->next, adest->next);
    }

  }

  return 0;
}

/*
 * Returns 1 if there is an identity conversion from src to dest, else 0 is 
 * returned. 
 */
int
identity_conversion(Type * src, Type * dest)
{
  return equal_types(src, dest);
}

/*
 * Returns 1 if there is a narrowing reference conversion from src to dest,
 * else 0 is returned.
 */
int
narrowing_reference(Type * src, Type * dest)
{
  ArrayType *asrc,
  *adest;

  if (dest->tag == CLASS_TYPE) {
    if (src->tag == CLASS_TYPE &&
        isSuper((ClassType *) src, (ClassType *) dest) == 1)
      return 1;
  } else if (dest->tag == ARRAY_TYPE) {

    if (src == &getClass(OBJECT_NAME)->super)
      return 1;

    if (src->tag == ARRAY_TYPE) {
      asrc = (ArrayType *) src;
      adest = (ArrayType *) dest;
      return narrowing_reference(asrc->next, adest->next);
    }
  }

  return 0;
}

int
assign_convertable(Type * src, Type * dest)
{
  assert(src != NULL);
  assert(dest != NULL);

  if (identity_conversion(src, dest) == 1 ||
      widening_reference(src, dest) == 1)
    return 1;

  return 0;
}


int
meth_convertable(Type * src, Type * dest)
{
  assert(src != NULL);
  assert(dest != NULL);

  if (identity_conversion(src, dest) == 1 ||
      widening_reference(src, dest) == 1)
    return 1;

  return 0;
}

int
cast_convertable(Type * src, Type * dest)
{
  assert(src != NULL);
  assert(dest != NULL);

  if (identity_conversion(src, dest) == 1 ||
      widening_reference(src, dest) == 1 ||
      narrowing_reference(src, dest) == 1)
    return 1;

  return 0;
}
