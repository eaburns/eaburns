/*
 * error.c implementation of the error function
 * Ethan Burns -- Created <unknown>
 */
#include <stdlib.h>
#include <stdio.h>
#include "error.h"

int error_count = 0;

void
error(const char *msg, AstNode * node)
{
  Expr *expr;

  error_count++;
  fprintf(stderr, "Error on line %d: %s\n", node->line, msg);

  if (isExpr(node) == 1) {
    expr = (Expr *) node;
    expr->type = makeErrorType();
  }
}
