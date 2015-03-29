/*
 * analyze.h declares functions for doing symantic analysis of an Ast.
 * Ethan Burns -- Created Wed Mar  1 21:06:39 2006
 */
#ifndef _ANALYZE_H_
#define _ANALYZE_H_

#include "ast.h"

/*
 * Analyzes an ast node for symantics.  This recurses on all children of
 * the given node.  The return value of the analyze function is the new
 * head node for the tree given by `n'. 
 */
AstNode *analyze(AstNode * n);

#endif                          /* !_ANALYZE_H_ */
