/*
 * conversion.h declares functions for checking conversions.
 * Ethan Burns -- Created Wed Mar  1 21:35:00 2006
 */
#ifndef _CONVERSION_H_
#define _CONVERSION_H_

#include "type.h"

/*
 * Returns 1 if there is an indentity conversion from src to dest.  If not then
 * 0 is returned.
 */
int identity_conversion(Type * src, Type * dest);

/*
 * Returns 1 if there is a widening reference conversion from src to dest.  
 * If not then 0 is returned.
 */
int widening_reference(Type * src, Type * dest);

/*
 * Returns 1 if there is a narrowing reference conversion from src to dest.  
 * If not then 0 is returned.
 */
int narrowing_reference(Type * src, Type * dest);

/*
 * Returns 1 if src can be converted to dest by assignment conversion.  If 
 * this conversion is not valid, 0 is returned. 
 */
int assign_convertable(Type * src, Type * dest);

/*
 * Returns 1 if src can be converted to dest by method invocation conversion.
 * If this conversion is not valid, 0 is returned.
 */
int meth_convertable(Type * src, Type * dest);

/*
 * Returns 1 if src can be cast converted to dest, else 0 is returned.
 */
int cast_convertable(Type * src, Type * dest);

#endif                          /* !_CONVERSION_H_ */
