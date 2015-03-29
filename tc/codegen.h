/*
 * codegen.h Declarations for the codegen AST traversal routines.
 * Ethan Burns -- Created Wed Mar 29 14:18:54 2006
 */
#ifndef _CODEGEN_H_
#define _CODEGEN_H_

#include <stdio.h>
#include "ast.h"

/*
 * Generates intel x86 assembly for a T program to stdout.
 */
void output_code(AstNode *root, FILE *outfile);

#endif /* !_CODEGEN_H_ */
