/*
 * error.h declaration of the error function.
 * Ethan Burns -- Created <unknown>
 */
#ifndef _ERROR_H_
#define _ERROR_H_

#include "ast.h"

/*
 * Number of errors encountered while doing symantic check. 
 */
extern int error_count;

void error(const char *msg, AstNode * node);

#endif                          /* !_ERROR_H_ */
