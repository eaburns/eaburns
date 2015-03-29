/*
 * analyze.c implementation for the analyzation of semantics.
 * Ethan Burns -- Created Wed Mar  1 21:35:00 2006
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <errno.h>
#include <string.h>
#include "analyze.h"
#include "type.h"
#include "conversion.h"
#include "error.h"
#include "symtab.h"
#include "class.h"

/*
 * Set to 1 if we are currently analyzing in a main block. 
 */
static int main_block = 0;

/*
 * Set to 1 if we are currently analyzing a negation.
 */
static int negation = 0;

/*
 * Set to 1 if we are currently analyzing the parameters of a constructor
 * invocation.
 */
static int const_invoc = 0;

/*
 * Number of currently declared destructors of a class.
 */
static int destructors = 0;

/*
 * Set to 1 if we are in a constructor or a destructor.
 */
static int const_dest = 0;

/* Returns 1 if a is more specific than b, else 0 is 
 * returned. */
static unsigned int
moreSpecific(MethodType *a, MethodType *b)
{
	ParamType *p, *q;	

	for(p = a->params, q = b->params;
      p != NULL && q != NULL; 
      p = p->next, q = q->next) {

			if(assign_convertable(p->type, q->type) != 1) 
				return 0;
	}

	/* Parameter lists at this point MUST have the same
   * number of parameters. */
	assert(p == NULL && q == NULL);

	return 1;
}

/*
 * Gets a list of the ``most specific methods'' from the 
 * given list of methods.
 */
static List *
getMostSpecific(List *meths)
{
	List *p, *j;
  List *retval = NULL;

	/* Check every method against every other method. */
	for(p = meths; p != NULL; p = p->next) {
		for(j = meths; j != NULL; j = j->next) {
			if(p == j) continue;

			if(moreSpecific((MethodType*)p->data, 
											(MethodType*)j->data) == 1) {
				retval = list_add(retval, p->data);
			}
		}
	}

	return retval;
}

static AstNode *
analyzeExprId(AstNode * n)
{
  ExprId *exprid;
  Type *type;
  Deref *deref;
  ClassType *class_type;
  MethodType *method_type;
  ParamType *param_type;

  assert(n->type == EXPRID_NODE);
  exprid = (ExprId *) n;

  /*
   * If we are in a main block, this *MUST* be a main variable.
   */
  if (main_block == 1) {
    type = lookupType(exprid->name);

    if (type == NULL) {
      error("symbol name expected", &exprid->super.super);
      return n;
    }

    exprid->super.type = type;
    exprid->super.super.type = MAINVAR_NODE;

  } else {                      /* Else *MUST* be in a class decl (4.4.4.1 
                                 * ``Otherwise...''). */

    /*
     * Grab the current class.
     */
    class_type = getCurClass();
    if (class_type == NULL) {   /* Not even sure what this means. */
      error("", &exprid->super.super);
      return n;
    }

    /*
     * First check method parameters, if we are in a method.
     */
    method_type = getCurMethod();
    if (method_type != NULL) {
      param_type = getParameter(exprid->name, method_type);

    } else {
      param_type = NULL;
    }

    /*
     * If this exprid is not a method parameter, search the fields.
     */
    if (param_type == NULL) {

      type = searchFieldType(exprid->name, class_type);
      if (type != NULL && const_invoc == 1) {
        error("field used in arguments of constructor invocation",
              &exprid->super.super);
        return n;
      }

      if (type == NULL) {
        error("field name expected", &exprid->super.super);
        return n;
      }

      exprid->super.super.type = FIELD_NODE;

    } else {
      type = param_type->type;
      exprid->super.super.type = PARAM_NODE;
    }

    exprid->super.type = type;
  }

  /*
   * Tack on a dereference node. 
   */
  deref = (Deref *) makeDeref(type, n, exprid->super.super.line);

  return &deref->super.super;
}

static AstNode *
analyzeMainVarDecl(AstNode * n)
{
  VarDecl *vardecl;
  Type *type;
  Type *t;                      /* Temp type for current declarator. */
  Seq *p;

  assert(n->type == MAINVARDECL_NODE);
  vardecl = (VarDecl *) n;

  /*
   * Set the type.
   */
  type = buildType(vardecl->type);

  /*
   * Analyze the declarators.
   */
  p = (Seq *) vardecl->decl;
  assert(p->super.type == SEQ_NODE);

  while (p != NULL) {

    /*
     * There may be an array, if so add the dims to the type.
     */
    t = copyArrays(type);
    t = appendArray(t, (ArrayType *) getDims(p->data));

    /*
     * Look up the type of the identifier, check for a duplicate.
     */
    if (lookupType(getIdent(p->data)) != NULL) {
      error("redeclaration of main variable", &vardecl->super);
      return n;
    }

    addSymbol(t, getIdent(p->data));

    p = (Seq *) p->next;
    if (p != NULL)
      assert(p->super.type == SEQ_NODE);
  }

  return n;
}

static AstNode *
analyzeMainFunc(AstNode * n)
{
  MainFunc *mainfunc;
  Seq *seq;
  IntLit *intlit;
  ReturnSt *ret;

  assert(n->type == MAINFUNC_NODE);
  mainfunc = (MainFunc *) n;

  main_block = 1;
  mainfunc->body = analyze(mainfunc->body);
  main_block = 0;

  /*
   * Tack on a return statement if there isn't already one. 
   */
  if (mainfunc->body == NULL) {
    intlit = (IntLit *) makeIntLit(makeLiteral("0", 0));
    intlit->super.type = makeIntType();
    ret = (ReturnSt *) makeReturnSt(&intlit->super.super, 0);
    mainfunc->body = makeSeq(&ret->super);
  } else {
    assert(mainfunc->body->type == SEQ_NODE);
    for (seq = (Seq *) mainfunc->body; seq->next != NULL;
         seq = (Seq *) seq->next);
    assert(seq->next == NULL);

    if (seq->data->type != RETURN_NODE) {
      intlit = (IntLit *) makeIntLit(makeLiteral("0", 0));
      intlit->super.type = makeIntType();
      ret = (ReturnSt *) makeReturnSt(&intlit->super.super, 0);
      appendSeq(makeSeq(&ret->super), &seq->super);
    }
  }

  return n;
}

AstNode *
analyzeAssignment(AstNode * n)
{
  BinaryOp *binop;
  Deref *deref;
  Expr *left,
    *right;

  assert(n->type == BINARYOP_NODE);
  binop = (BinaryOp *) n;
  assert(binop->op == ASI_OP);

  binop->left = analyze(binop->left);
  binop->right = analyze(binop->right);

  /*
   * Make sure there is a deref on the left side (ensures a variable). 
   */
  if (binop->left->type != DEREF_NODE) {

    /* If the error is already reported, gracefully return. */
    if( ((Expr*)binop->left)->type->tag == ERROR_TYPE) {
      binop->super.type = makeErrorType();
      return n;
    }

    error("assignment left-hand-side is not a variable",
	  &binop->super.super);
    return n;
  }

  /*
   * ... now remove that deref (assign to variable, not value). 
   */
  deref = (Deref *) binop->left;
  binop->left = deref->next;

  left = (Expr *) binop->left;
  right = (Expr *) binop->right;

  if (left->type->tag == ERROR_TYPE || right->type->tag == ERROR_TYPE) {
    binop->super.type = makeErrorType();
    return n;
  }

  if (assign_convertable(right->type, left->type) == 0) {
    error("invalid assignment conversion", &binop->super.super);
    return n;
  }

  binop->super.type = left->type;

  return n;
}

static AstNode *
analyzeEquality(AstNode * n)
{
  BinaryOp *binop;
  Expr *left,
    *right;

  assert(n->type == BINARYOP_NODE);
  binop = (BinaryOp *) n;
  assert(binop->op == EQU_OP);

  binop->left = analyze(binop->left);
  binop->right = analyze(binop->right);
  left = (Expr *) binop->left;
  right = (Expr *) binop->right;

  if (left->type->tag == ERROR_TYPE || right->type->tag == ERROR_TYPE) {
    binop->super.type = makeErrorType();
    return n;
  }

  if ((left->type->tag == INT_TYPE && right->type->tag != INT_TYPE) ||
      (right->type->tag == INT_TYPE && left->type->tag != INT_TYPE)) {

    /* One type is int, but not both. */
    error("invalid types in comparison", &binop->super.super);
    return n;

  } 
#if 0
  else if ((isReference(left->type) != isReference(right->type)) &&
	   (isReference(left->type) && right->type->tag != NULL_TYPE) &&
	   (left->type->tag != NULL_TYPE && isReference(right->type))) {
    /* One type is a reference type and the other is not a reference or 
     * the null type... I think this is covered by the above case. */
    error("invalid types in comparison", &binop->super.super);
    return n;
  }
#endif /* 0 */

  binop->super.type = makeIntType();

  return n;
}

static AstNode *
analyzeComparison(AstNode * n)
{
  BinaryOp *binop;
  Expr *left,
    *right;

  assert(n->type == BINARYOP_NODE);
  binop = (BinaryOp *) n;
  assert(binop->op == LES_OP || binop->op == GRT_OP);

  binop->left = analyze(binop->left);
  binop->right = analyze(binop->right);
  left = (Expr *) binop->left;
  right = (Expr *) binop->right;

  if (left->type->tag == ERROR_TYPE || right->type->tag == ERROR_TYPE) {
    binop->super.type = makeErrorType();
    return n;
  }

  if (left->type->tag != INT_TYPE || right->type->tag != INT_TYPE) {
    error("operand of compairson not of integer type",
          &binop->super.super);
    return n;
  }

  binop->super.type = makeIntType();

  return n;
}

static AstNode *
analyzeArithmetic(AstNode * n)
{
  BinaryOp *binop;
  Expr *left,
    *right;

  assert(n->type == BINARYOP_NODE);
  binop = (BinaryOp *) n;
  assert(binop->op == MUL_OP ||
         binop->op == DIV_OP ||
         binop->op == ADD_OP || binop->op == SUB_OP);

  binop->left = analyze(binop->left);
  binop->right = analyze(binop->right);
  left = (Expr *) binop->left;
  right = (Expr *) binop->right;

  if (left->type->tag == ERROR_TYPE || right->type->tag == ERROR_TYPE) {
    binop->super.type = makeErrorType();
    return n;
  }

  if (left->type->tag != INT_TYPE || right->type->tag != INT_TYPE) {
    error("operand of arithmetic operator not of integer type",
          &binop->super.super);
    return n;
  }

  binop->super.type = makeIntType();

  return n;
}

static AstNode *
analyzeBinaryOp(AstNode * n)
{
  BinaryOp *binop;

  assert(n->type == BINARYOP_NODE);
  binop = (BinaryOp *) n;

  switch (binop->op) {
  case ASI_OP:
    return analyzeAssignment(n);
  case EQU_OP:
    return analyzeEquality(n);
  case LES_OP:
    return analyzeComparison(n);
  case GRT_OP:
    return analyzeComparison(n);
  case ADD_OP:
    return analyzeArithmetic(n);
  case SUB_OP:
    return analyzeArithmetic(n);
  case MUL_OP:
    return analyzeArithmetic(n);
  case DIV_OP:
    return analyzeArithmetic(n);
  default:
    assert("UnaryOp tag for BinaryOp structure" == 0);
  }

	return NULL;
}

static AstNode *
analyzeNot(AstNode * n)
{
  UnaryOp *unaryop;
  Expr *left;

  assert(n->type == UNARYOP_NODE);
  unaryop = (UnaryOp *) n;
  assert(unaryop->op == NOT_OP);

  unaryop->left = analyze(unaryop->left);
  left = (Expr *) unaryop->left;

  if (left->type->tag == ERROR_TYPE) {
    unaryop->super.type = makeErrorType();
    return n;
  }

  if (left->type->tag != INT_TYPE) {
    error("operand of unary ! operator not of integer type",
          &unaryop->super.super);
    return n;
  }

  unaryop->super.type = makeIntType();

  return n;
}

static AstNode *
analyzeNeg(AstNode * n)
{
  UnaryOp *unaryop;
  Expr *left;

  assert(n->type == UNARYOP_NODE);
  unaryop = (UnaryOp *) n;
  assert(unaryop->op == NEG_OP);

  /*
   * If the expression being negated is an integer literal, set the
   * negation flag so we can correctly check the integer's max size. Then 
   * throw out the negation node since the integer literal will hold the
   * information about being a negation. 
   */
  if (unaryop->left->type == INTLIT_NODE) {
    negation = 1;
    /*
     * Don't actually free this node, that will be freed up with the AST
     * pool. 
     */
    unaryop->left = analyze(unaryop->left);
    negation = 0;

    return unaryop->left;
  }

  unaryop->left = analyze(unaryop->left);

  left = (Expr *) unaryop->left;

  if (left->type->tag == ERROR_TYPE) {
    unaryop->super.type = makeErrorType();
    return n;
  }

  if (left->type->tag != INT_TYPE) {
    error("operand of unary - operator not of integer type",
          &unaryop->super.super);
    return n;
  }

  unaryop->super.type = makeIntType();

  return n;
}

static AstNode *
analyzeUnaryOp(AstNode * n)
{
  UnaryOp *unaryop;

  assert(n->type == UNARYOP_NODE);
  unaryop = (UnaryOp *) n;

  switch (unaryop->op) {
  case NOT_OP:
    return analyzeNot(n);
  case NEG_OP:
    return analyzeNeg(n);
  default:
    assert("BinaryOp tag for UnaryOp structure" == 0);
  }

	return NULL;
}

static AstNode *
analyzeNullLit(AstNode * n)
{
  Expr *expr;

  assert(n->type == NULLLIT_NODE);
  expr = (Expr *) n;

  expr->type = makeNullType();

  return n;
}

static AstNode *
analyzeIntLit(AstNode * n)
{
  char *newStr;
  IntLit *intlit;

  assert(n->type == INTLIT_NODE);
  intlit = (IntLit *) n;

  if (negation == 1) {          /* If we are after a negation; tack on a
                                 * '-' */
    newStr = malloc(sizeof(char) * strlen(intlit->str) + 2);
    newStr[0] = '-';
    strcat(newStr, intlit->str);
    free(intlit->str);
    intlit->str = newStr;
  }

  errno = 0;
  strtol(intlit->str, NULL, 10);
  if (errno == ERANGE) {
    error("invalid integer literal", &intlit->super.super);
    return n;
  }

  intlit->super.type = makeIntType();

  return n;
}

/*
 * Returns 1 if there is an error type in a parameter list, else 0 is 
 * returned.
 */
static int
errorInParams(AstNode * n)
{
  Seq *seq;
  Expr *expr;

  if (n == NULL)
    return 0;

  assert(n->type == SEQ_NODE);
  seq = (Seq *) n;

  for (; seq != NULL; seq = (Seq *) seq->next) {
    assert(isExpr(seq->data) == 1);
    expr = (Expr *) seq->data;

    if (expr->type->tag == ERROR_TYPE)
      return 1;
  }

  return 0;
}

static AstNode *
analyzeMethodInvoc(AstNode * n)
{
#ifdef PRINT_MATCHING_METHODS
  List *l;
#endif                          /* PRINT_MATCHING_METHODS */
  List *meths, *more_specific;
  MethodInvoc *methinvoc;
  Expr *target;
  Type *ttype;                  /* Target type. */

  assert(n->type == METHODINVOC_NODE);
  methinvoc = (MethodInvoc *) n;

  /*
   * Check the target. 
   */
  if(methinvoc->target == NULL)
    methinvoc->target = makeThis(0);

  methinvoc->target = analyze(methinvoc->target);

  assert(methinvoc->target != NULL);
  assert(isExpr(methinvoc->target) == 1);

  target = (Expr *) methinvoc->target;

  if (target->type->tag == ERROR_TYPE) {
    methinvoc->super.type = makeErrorType();
    return n;
  }

  if (target->super.type == SUPER_NODE) {
    ttype = &getCurClass()->parent->super;

  } else {
    ttype = target->type;

  }

  assert(ttype != NULL);

	
  if (ttype->tag != CLASS_TYPE &&
			ttype->tag != ARRAY_TYPE) {
    error("method invocation target is not a class",
          &methinvoc->super.super);
    return n;
  }

  /*
   * Check the arguments. 
   */
  methinvoc->args = analyze(methinvoc->args);

  if (errorInParams(methinvoc->args) == 1) {
    methinvoc->super.type = makeErrorType();
    return n;
  }

  if (methinvoc->args != NULL)
    assert(methinvoc->args->type == SEQ_NODE);

  /*
   * Find matching methods.
   */
	if(ttype->tag == ARRAY_TYPE) {
		if(strcmp(methinvoc->name, "equals") != 0) {
    	error("method not found, or invalid parameter conversion",
     	     &methinvoc->super.super);
    	return n;
		}
		meths = searchMethods(methinvoc->name, (Seq *) methinvoc->args,
													getClass(OBJECT_NAME));	
	} else {
		meths = searchMethods(methinvoc->name, (Seq *) methinvoc->args,
                       (ClassType *) ttype);
	}

  if (meths == NULL) {
    error("method not found, or invalid parameter conversion",
          &methinvoc->super.super);
    return n;
  }

  /*
   * print matching methods.
   */
#ifdef PRINT_MATCHING_METHODS
  for (l = meths; l != NULL; l = l->next) {
    printMethod((MethodType *) l->data);
    printf("\n");
  }
#endif                          /* PRINT_MATCHING_METHODS */

#if 0
  /*
   * Find the best fitting method. XXX -- For now we are assuming no
   * overloading so we skip this. 
   */
  if (meths->next != NULL) {
    error("ambiguous method invocation", &methinvoc->super.super);
    return n;
  }
#else
  if (meths->next != NULL) {
		more_specific = getMostSpecific(meths);
		list_free(meths);

		if(more_specific == NULL || more_specific->next != NULL) {
    	error("ambiguous method invocation", &methinvoc->super.super);
    	return n;
		}
  	methinvoc->method_type = ((MethodType*)more_specific->data);
		list_free(more_specific);
	} else {
  	methinvoc->method_type = ((MethodType*)meths->data);
		list_free(meths);
	}
#endif /* 0 */

  methinvoc->super.type = methinvoc->method_type->type;
  /* methinvoc->super.type = ((MethodType *) meths->data)->type; */

  return n;
}

static AstNode *
analyzeConstInvoc(AstNode * n)
{
#ifdef PRINT_MATCHING_METHODS
  List *l;
#endif                          /* PRINT_MATCHING_METHODS */
  List *consts;
	List *more_specific;
  ConstInvoc *constinvoc;
  MethodType *mtype,
    *mp;
  Expr *target;
  Type *ttype;

  assert(n->type == CONSTINVOC_NODE);
  constinvoc = (ConstInvoc *) n;

  /*
   * Check the target. 
   */
  constinvoc->target = analyze(constinvoc->target);

  assert(constinvoc->target != NULL);
  assert(constinvoc->target->type == SUPER_NODE ||
         constinvoc->target->type == THIS_NODE);

  target = (Expr *) constinvoc->target;

  if (target->type == NULL || target->type->tag == ERROR_TYPE) {
    constinvoc->super.type = makeErrorType();
    return n;
  }

  if (target->super.type == SUPER_NODE) {
    ttype = &getCurClass()->parent->super;
  } else if (target->super.type == THIS_NODE) {
    ttype = &getCurClass()->super;
  } else {
    assert("Not possible" == 0);
  }

  assert(ttype != NULL);
  assert(ttype->tag == CLASS_TYPE);

  /*
   * Check the arguments. 
   */
  const_invoc = 1;
  constinvoc->args = analyze(constinvoc->args);
  const_invoc = 0;

  if (errorInParams(constinvoc->args) == 1) {
    constinvoc->super.type = makeErrorType();
    return n;
  }

  if (constinvoc->args != NULL)
    assert(constinvoc->args->type == SEQ_NODE);

  /*
   * Find matching methods.
   */
  consts =
    searchConstructor((Seq *) constinvoc->args, (ClassType *) ttype);

  if (consts == NULL) {
    error("constructor not found", &constinvoc->super.super);
    return n;
  }

  /*
   * print matching constructors.
   */
#ifdef PRINT_MATCHING_METHODS
  for (l = consts; l != NULL; l = l->next) {
    printMethod((MethodType *) l->data);
    printf("\n");
  }
#endif                          /* PRINT_MATCHING_METHODS */

#if 0
  /*
   * Find the best fitting method. XXX -- For now we are assuming no
   * overloading so we skip this. 
   */
  if (consts->next != NULL) {
    error("ambiguous constructor invocation", &constinvoc->super.super);
    return n;
  }
#else
  if (consts->next != NULL) {
		more_specific = getMostSpecific(consts);
		list_free(consts);

		if(more_specific == NULL || more_specific->next != NULL) {
    	error("ambiguous constructor invocation", &constinvoc->super.super);
    	return n;
		}
  	mtype = (MethodType*)more_specific->data;
		list_free(more_specific);
	} else {
  	mtype = (MethodType*)consts->data;
		list_free(consts);
	}
#endif /* 0 */

  if (target->super.type == THIS_NODE) {
    getCurMethod()->invokedConst = mtype;
    for (mp = mtype->invokedConst; mp != NULL; mp = mp->invokedConst) {
      if (mp == mtype) {
        error("cyclical constructor invocation", &constinvoc->super.super);
        return n;
      }
    }

  }

  /* This should always be null... not sure why its here. */
  constinvoc->super.type = mtype->type;

  /* This is more important than the previous info. */
  constinvoc->cons = mtype;

  return n;
}

static AstNode *
analyzeFieldAccess(AstNode * n)
{
  FieldAccess *facc;
  Expr *target;
  Deref *deref;
  Type *type;
  ClassType *class_type;

  assert(n->type == FIELDACCESS_NODE);
  facc = (FieldAccess *) n;

  /*
   * Analyze the target class.
   */
  facc->target = analyze(facc->target);
  assert(isExpr(facc->target) == 1);
  target = (Expr *) facc->target;

  if (target->type->tag == ERROR_TYPE) {
    facc->super.type = makeErrorType();
    return n;
  }

  /*
   * Get the ClassType from the target. 
   */
  if (target->super.type == SUPER_NODE) {
    class_type = (ClassType *) target->type;

  } else if (target->super.type == THIS_NODE) {
    class_type = (ClassType *) target->type;

  } else {
    if (isReference(target->type) == 0) {
      error("target of field access is not a reference type",
            &facc->super.super);
      return n;
    }

		if(target->type->tag == ARRAY_TYPE) {
			/* We have an array, the access *BETTER* be for 
       * length */
			if(strcmp(facc->name, "length") != 0) {
    		error("field does not exist", &facc->super.super);
    		return n;
			} else {
				n->type = LENGTHACC_NODE;	
				facc->super.type = makeIntType();

  			deref = (Deref *) 
					makeDeref(facc->super.type, n, facc->super.super.line);
  			return &deref->super.super;
			}
		}

    class_type = (ClassType *) target->type;
  }

  /*
   * Look for the type of the field in the target class. 
   */
  type = searchFieldType(facc->name, class_type);

  if (type == NULL) {
    error("field does not exist", &facc->super.super);
    return n;
  }

  facc->super.type = type;

  /*
   * Throw on the deref. 
   */
  deref = (Deref *) makeDeref(facc->super.type, n, facc->super.super.line);

  return &deref->super.super;
}

static AstNode *
analyzeSuper(AstNode * n)
{
  Expr *expr;

  /*
   * Super can not be used in the parameters of a constructor invocation.
   */
  if (const_invoc == 1) {
    error("keyword `super' used in argument of constructor invocation", n);
    return n;
  }

  assert(n->type == SUPER_NODE);
  expr = (Expr *) n;

  if (getCurClass() == NULL) {
    error("'super' keyword used outside of a class declaration",
          &expr->super);
    return n;
  }

  expr->type = &getCurClass()->parent->super;

  if(expr->type == NULL)
    expr->type = makeErrorType();

  return n;
}

static AstNode *
analyzeThis(AstNode * n)
{
  Expr *expr;

  /*
   * This can not be used in the parameters of a constructor invocation.
   */
  if (const_invoc == 1) {
    error("keyword `this' used in argument of constructor invocation", n);
    return n;
  }

  assert(n->type == THIS_NODE);
  expr = (Expr *) n;

  if (getCurClass() == NULL) {
    error("'this' keyword used outside of a class declaration",
          &expr->super);
    return n;
  }

  expr->type = &getCurClass()->super;

  return n;
}

static AstNode *
analyzeClass(AstNode * n)
{
  Class *class;
  MethodType *mp;
  ClassType *class_type;

  assert(n->type == CLASS_NODE);
  class = (Class *) n;

  /*
   * class->ident = analyze(class->ident); class_type = getClass(
   * ((NameId*)class->ident)->name ); 
   */
  class_type = getClass(class->name);

  assert(class_type != NULL);

  if (((Type *) class_type)->tag == ERROR_TYPE)
    return n;

  /*
   * If we have a default constructor, make sure that our super class has
   * a no argument constructor. 
   */
  if (class_type->parent != NULL && 
      class_type->cons->def == 1) {     /* Always first and only
                                         * constructor. */
    for (mp = class_type->parent->cons;
	 mp != NULL && mp->params != NULL; mp = mp->next);
    
    if (mp == NULL) {
      error("default constructor used where parent has no zero argument"
	    " constructor", &class->super);
      return n;
    }
  }
  
  destructors = 0;

  setCurrentClass(class_type);
  class->body = analyze(class->body);
  setCurrentClass(NULL);

  /*
   * It is an error if a class has multiple destructors.
   */
  if (destructors > 1) {
    error("class has multiple destructors", &class->super);
    return n;
  } else if (destructors == 0) {

    if(class->body != NULL) {
      appendSeq(makeDefaultDestructor(class_type), class->body);
    }
  }

  return n;
}

static AstNode *
analyzeMethodDecl(AstNode * n)
{
  MemberDecl *mdecl;
  MethodType *method_type;
  MethodType *pmeth;
  ClassType *parent, *cur_class;
  AstNode *retExpr, *invoc;
  ReturnSt *ret;
  Seq *seq;

  assert(n->type == METHDECL_NODE || n->type == CONSTDECL_NODE);
  mdecl = (MemberDecl *) n;

  /*
   * Analyze children (These probably will do nothing, but oh well for now).
   */
  mdecl->type = analyze(mdecl->type);   /* NULL for a constructor. */
  mdecl->decl = analyze(mdecl->decl);

  /*
   * See if this method is an error or not. 
   */
  if (mdecl->t->tag == ERROR_TYPE) {
    return n;
  }

  assert(mdecl->t->tag == METHOD_TYPE
         || mdecl->t->tag == CONSTRUCTOR_TYPE);
  method_type = (MethodType *) mdecl->t;

  if (mdecl->super.type == CONSTDECL_NODE) {
    const_dest = 1;
  } else {
    /*
     * Make sure we don't override and change a return type. 
     */
    cur_class = getCurClass();
    assert(cur_class != NULL);
    for (parent = cur_class->parent; parent != NULL;
         parent = parent->parent) {
      for (pmeth = parent->methods; pmeth != NULL; pmeth = pmeth->next) {

        if (same_sigs(pmeth, method_type) == 1 &&
            method_type->type != pmeth->type) {
          error("overriding method changes return type", &mdecl->super);
          return n;
        }

      }
    }
  }

  /*
   * Tack on a return statement if there isn't already one. 
   */
  if (mdecl->super.type == CONSTDECL_NODE) {
    retExpr = NULL;
  } else if (method_type->type->tag == INT_TYPE) {
    retExpr = makeIntLit(makeLiteral("0", 0));
    ((Expr *) retExpr)->type = makeIntType();
  } else {
    retExpr = makeNullLit(0);
    ((Expr *) retExpr)->type = makeNullType();
  }

  if (mdecl->body == NULL) {
    ret = (ReturnSt *) makeReturnSt(retExpr, 0);
    mdecl->body = makeSeq(&ret->super);
  } else {
    assert(mdecl->body->type == SEQ_NODE);
    for (seq = (Seq *) mdecl->body; seq->next != NULL;
         seq = (Seq *) seq->next);
    assert(seq->next == NULL);

    if (seq->data->type != RETURN_NODE) {
      ret = (ReturnSt *) makeReturnSt(retExpr, 0);
      seq->next = makeSeq(&ret->super);
    }
  }

  /*
   * Make sure a constructor body starts with a constructor
   * invocation... if we are not in Object.
   */
  cur_class = getCurClass();
  if(mdecl->super.type == CONSTDECL_NODE && 
     strcmp(cur_class->name, OBJECT_NAME) != 0) {

    assert(mdecl->body->type == SEQ_NODE);
    seq = (Seq*)mdecl->body;

    invoc = (AstNode*)seq->data;
    if(invoc->type != CONSTINVOC_NODE) {
      mdecl->body = 
			appendSeq(mdecl->body, makeSeq(makeConstInvoc(makeSuper(0),NULL, 0)));
    }
   	 
  }

  mdecl->mtype = method_type;
  setCurrentMethod(method_type);
  mdecl->body = analyze(mdecl->body);
  setCurrentMethod(NULL);
  const_dest = 0;

  return n;
}

static AstNode *
analyzeDestructor(AstNode * n)
{
  Destructor *des;
  Seq *seq;
  ReturnSt *ret;

  assert(n->type == DESTRUCTOR_NODE);
  des = (Destructor *) n;

  const_dest = 1;
  des->body = analyze(des->body);
  const_dest = 0;

  /*
   * Tack on a return statement if there isn't already one. 
   */
  if (des->body == NULL) {
    ret = (ReturnSt *) makeReturnSt(NULL, 0);
    des->body = makeSeq(&ret->super);
  } else {
    assert(des->body->type == SEQ_NODE);
    for (seq = (Seq *) des->body; seq->next != NULL;
         seq = (Seq *) seq->next);
    assert(seq->next == NULL);

    if (seq->data->type != RETURN_NODE) {
      ret = (ReturnSt *) makeReturnSt(NULL, 0);
      seq->next = makeSeq(&ret->super);
    }
  }

  /*
   * Make sure the destructor has the correct name.
   */
  if (strcmp(des->name, getCurClass()->name) != 0) {
    error("destructor name not the same as class name", &des->super);
    return n;
  }

  destructors++;

  return n;
}

static AstNode *
analyzeIfElse(AstNode * n)
{
  IfElseSt *ifelse;
  Expr *expr;

  assert(n->type == IFELSE_NODE);
  ifelse = (IfElseSt *) n;

  ifelse->expr = analyze(ifelse->expr);
  ifelse->tstat = analyze(ifelse->tstat);
  ifelse->fstat = analyze(ifelse->fstat);

  assert(isExpr(ifelse->expr) == 1);
  expr = (Expr *) ifelse->expr;

  if (expr->type->tag == ERROR_TYPE) {
    return n;
  }

  if (expr->type->tag != INT_TYPE) {
    error("expression of if-then-else statement is not of type int",
          &ifelse->super);
    return n;
  }

  return n;
}

static AstNode *
analyzeWhile(AstNode * n)
{
  WhileSt *wh;
  Expr *expr;

  assert(n->type == WHILE_NODE);
  wh = (WhileSt *) n;

  wh->expr = analyze(wh->expr);
  wh->stats = analyze(wh->stats);

  assert(isExpr(wh->expr) == 1);
  expr = (Expr *) wh->expr;

  if (expr->type->tag == ERROR_TYPE) {
    return n;
  }

  if (expr->type->tag != INT_TYPE) {
    error("expression of while statement is not of type int", &wh->super);
    return n;
  }

  return n;
}

static AstNode *
analyzeReturn(AstNode * n)
{
  ReturnSt *ret;
  Expr *expr;
  MethodType *method_type;

  assert(n->type == RETURN_NODE);
  ret = (ReturnSt *) n;

  ret->expr = analyze(ret->expr);

  if (ret->expr == NULL) {

    if (const_dest == 0) {
      error("return statement without an expression from "
            "method or main function", &ret->super);
      return n;
    }

  } else {
    assert(isExpr(ret->expr) == 1);
    expr = (Expr *) ret->expr;

    if (expr->type->tag == ERROR_TYPE)
      return n;

    if (main_block == 1) {
      if (expr->type->tag != INT_TYPE) {
        error("return from main not of type int", &ret->super);
        return n;
      }

    } else {

      method_type = getCurMethod();

      if (const_dest == 1) {
        error("return with an expression from constructor or destructor",
              &ret->super);
        return n;
      }

      /*
       * This would mean we are in main, or the const_dest flag didn't
       * catch a destructor. 
       */
      assert(method_type != NULL);

      if (assign_convertable(expr->type, method_type->type) != 1) {
        error("type of return is not convertable to method return type",
              &ret->super);
        return n;
      }

    }
  }

  return n;
}

static AstNode *
analyzeDelete(AstNode * n)
{
  DeleteSt *delete;
  Expr *expr;

  assert(n->type == DELETE_NODE);
  delete = (DeleteSt *) n;

  delete->expr = analyze(delete->expr);

  assert(isExpr(delete->expr) == 1);
  expr = (Expr *) delete->expr;

  if (expr->type->tag == ERROR_TYPE)
    return n;

  if (isReference(expr->type) != 1) {
    error("delete expression is not of reference type", &delete->super);
    return n;
  }

  return n;
}

static AstNode *
analyzeOut(AstNode * n)
{
  OutSt *out;
  Expr *expr;

  assert(n->type == OUT_NODE);
  out = (OutSt *) n;

  out->expr = analyze(out->expr);

  assert(isExpr(out->expr) == 1);
  expr = (Expr *) out->expr;

  if (expr->type->tag == ERROR_TYPE)
    return n;

  if (expr->type->tag != INT_TYPE) {
    error("expression of out statement not of type int", &out->super);
    return n;
  }

  return n;
}

static AstNode *
analyzeClassCreate(AstNode * n)
{
#ifdef PRINT_MATCHING_METHODS
  List *l
#endif                          /* PRINT_MATCHING_METHODS */
  List * consts;
	List *more_specific;
  ClassCreate *classc;
  NameId *name;
  ClassType *class_type;

  assert(n->type == CLASSCREATE_NODE);
  classc = (ClassCreate *) n;

  classc->type = analyze(classc->type);
  classc->args = analyze(classc->args);

  assert(classc->type->type == NAMEID_NODE);
  name = (NameId *) classc->type;

  /*
   * Get our creation class type.
   */
  class_type = getClass(name->name);

  if (class_type == NULL) {
    error("class name expected", &classc->super.super);
    classc->super.type = makeErrorType();
    return n;
  }

  classc->super.type = &class_type->super;

  /*
   * Check for an error in our argument expressions.
   */
  if (errorInParams(classc->args) == 1) {
    classc->super.type = makeErrorType();
    return n;
  }

  if (classc->args != NULL)
    assert(classc->args->type == SEQ_NODE);

  /*
   * Find a matching constructor.
   */
  consts = searchConstructor((Seq *) classc->args, class_type);

  if (consts == NULL) {
    error("constructor not found", &classc->super.super);
    return n;
  }

  /*
   * print matching constructors.
   */
#ifdef PRINT_MATCHING_METHODS
  for (l = consts; l != NULL; l = l->next) {
    printMethod((MethodType *) l->data);
    printf("\n");
  }
#endif                          /* PRINT_MATCHING_METHODS */

#if 0
  /*
   * Find the best fitting method. XXX -- For now we are assuming no
   * overloading so we skip this. 
   */
  if (consts->next != NULL) {
    error("ambiguous constructor invocation", &classc->super.super);
    return n;
  }
#else
  if (consts->next != NULL) {
		more_specific = getMostSpecific(consts);
		list_free(consts);

		if(more_specific == NULL || more_specific->next != NULL) {
    	error("ambiguous constructor invocation", &classc->super.super);
    	return n;
		}
  	classc->cons = (MethodType*)more_specific->data;
		list_free(more_specific);
	} else {
  	classc->cons = (MethodType*)consts->data;
		list_free(consts);
	}
#endif /* 0 */

  return n;
}

static AstNode *
analyzeCast(AstNode * n)
{
  Cast *cast;
  Expr *expr;
  ExprId *exprid;
  Type *type;

  assert(n->type == CAST_NODE);
  cast = (Cast *) n;

  cast->expr = analyze(cast->expr);

  /*
   * Analyze the target type.
   */
  if (cast->type->type != EXPRID_NODE &&        /* Class name or */
      cast->type->type != ARRAY_NODE) { /* Array chain. */
    error("invalid target type operand to cast operator",
          &cast->super.super);
    return n;
  }

  if (cast->type->type == EXPRID_NODE) {
    exprid = (ExprId *) cast->type;
    type = &getClass(exprid->name)->super;

    if (type == NULL) {
      error("unknown symbol in target type operand of cast operator",
            &cast->super.super);
      return n;
    }
  } else {
    type = buildType(cast->type);
  }

  cast->super.type = type;
  cast->type = NULL; /* Cut off this exprid from the tree... we no longer
		      * need it, it will be freed with the AST pool later. */

  /*
   * Analyze the expression being cast.
   */
  assert(isExpr(cast->expr) == 1);
  expr = (Expr *) cast->expr;

  if (expr->type->tag == ERROR_TYPE)
    return n;

  if (cast_convertable(expr->type, cast->super.type) != 1) {
    error("invalid cast conversion", &cast->super.super);
    return n;
  }

  /*
   * Only narrowing reference conversions need code generated for them.
   */
  if(identity_conversion(expr->type, cast->super.type) == 1 ||
     widening_reference(expr->type, cast->super.type) == 1) {
    return cast->expr;
  }

  return n;
}

static AstNode *
analyzeFormalParam(AstNode * n)
{
  FormalParam *fparm;

  assert(n->type == FORMALPARAM_NODE);
  fparm = (FormalParam *) n;

  fparm->name = getIdent(fparm->decl);

  return n;
}

static AstNode *
analyzeConstDeclator(AstNode * n)
{
  MethDeclator *mdecl;

  assert(n->type == CONSTDECLATOR_NODE);
  mdecl = (MethDeclator *) n;

  mdecl->param = analyze(mdecl->param);

  return n;
}

static AstNode *
analyzeMethDeclator(AstNode * n)
{
  MethDeclator *mdecl;

  assert(n->type == METHDECLATOR_NODE);
  mdecl = (MethDeclator *) n;

  mdecl->param = analyze(mdecl->param);

  return n;
}

static AstNode *
analyzeSeq(AstNode *n)
{
  Seq *seq;

  assert(n->type == SEQ_NODE);
  seq = (Seq *) n;

  if(seq->data == NULL)
    return analyze(seq->next);

  seq->data = analyze(seq->data);
  seq->next = analyze(seq->next);

  return n;
}

static AstNode *
analyzeExprSt(AstNode *n)
{
  ExprSt *expr;

  assert(n->type == EXPRST_NODE);
  expr = (ExprSt*)n;

  expr->expr = analyze(expr->expr);

  return n;
}

static AstNode *
analyzeArrayCreate(AstNode *n)
{
	ArrayCreate *create;
	Seq *seq;
	unsigned int i;
	Type *t;

	assert(n->type == ARRAYCREATE_NODE);
	create = (ArrayCreate*)n;

	create->expr = analyze(create->expr);

	/* Compute the rank of the array we are creating. */
	create->rank = create->dims;
	for(seq = (Seq*)create->expr; seq != NULL; seq = (Seq*)seq->next) {
		/* Make sure we don't have an errored expression. */
		if(((Expr*)seq->data)->type->tag == ERROR_TYPE) {
			create->super.type = makeErrorType();
			return n;
		}
		
		/* By the way, make sure these are all of type int. */
		if(((Expr*)seq->data)->type->tag != INT_TYPE) {
			error("Dimension type is not int.", &create->super.super);	
			return n;
		}

		create->rank++;
	}

	/* Create the Type for the array being created. */
	t = buildType(create->type);
	for(i = 0; i < create->rank; i++)
		t = &makeArrayType(t)->super;	

/*
	printf("creating array: ");
	printType(t);
	printf("\n");
*/

	create->super.type = t;

	return n;
}

static AstNode *
analyzeArrayAccess(AstNode *n)
{
	ArrayAccess *access;	
	Expr *ref, *exp;

	assert(n->type == ARRAYACCESS_NODE);
	access = (ArrayAccess*)n;

	access->name = analyze(access->name);
	access->dexp = analyze(access->dexp);

	/* Check the reference expression. */
	assert(isExpr(access->name) == 1);
	ref = (Expr*)access->name;

	if(ref->type->tag == ERROR_TYPE) {
		access->super.type = makeErrorType();
		return n;
	}

	if(ref->type->tag != ARRAY_TYPE) {
		error("Reference is not of type array.",
					&access->super.super);
		return n;
	}

	/* Check the dimension expression. */
	assert(isExpr(access->dexp) == 1);
	exp = (Expr*)access->dexp;

	if(exp->type->tag == ERROR_TYPE) {
		access->super.type = makeErrorType();
		return n;
	}

	if(exp->type->tag != INT_TYPE) {
		error("Dimension expression not of type int.",
					&access->super.super);
		return n;
	}

	access->super.type = ((ArrayType*)ref->type)->next;

/*
	printf("access makes type: ");
	printType(access->super.type);
	printf("\n");
*/

	return makeDeref(access->super.type, &access->super.super,
                   access->super.super.line);
}

AstNode *
analyze(AstNode * n)
{
  if (n == NULL)
    return n;

  switch (n->type) {

  case SEQ_NODE:
    return analyzeSeq(n);

  case NULLLIT_NODE:
    return analyzeNullLit(n);

  case INTLIT_NODE:
    return analyzeIntLit(n);

  case INT_NODE:
    return n;

  case NAMEID_NODE:
    return n;

  case EXPRID_NODE:
    return analyzeExprId(n);

  case MAINVARDECL_NODE:
    return analyzeMainVarDecl(n);

  case MAINFUNC_NODE:
    return analyzeMainFunc(n);

  case CLASS_NODE:
    return analyzeClass(n);

  case CONSTDECL_NODE:
    return analyzeMethodDecl(n);

  case METHDECL_NODE:
    return analyzeMethodDecl(n);

  case BINARYOP_NODE:
    return analyzeBinaryOp(n);

  case UNARYOP_NODE:
    return analyzeUnaryOp(n);

  case SUPER_NODE:
    return analyzeSuper(n);

  case THIS_NODE:
    return analyzeThis(n);

  case FIELDACCESS_NODE:
    return analyzeFieldAccess(n);

  case METHODINVOC_NODE:
    return analyzeMethodInvoc(n);

  case CONSTDECLATOR_NODE:
    return analyzeConstDeclator(n);

  case FIELDDECL_NODE:
    return n;

  case METHDECLATOR_NODE:
    return analyzeMethDeclator(n);

  case CONSTINVOC_NODE:
    return analyzeConstInvoc(n);

  case FORMALPARAM_NODE:
    return analyzeFormalParam(n);

  case DESTRUCTOR_NODE:
    return analyzeDestructor(n);

  case IFELSE_NODE:
    return analyzeIfElse(n);

  case WHILE_NODE:
    return analyzeWhile(n);

  case RETURN_NODE:
    return analyzeReturn(n);

  case DELETE_NODE:
    return analyzeDelete(n);

  case OUT_NODE:
    return analyzeOut(n);

  case CLASSCREATE_NODE:
    return analyzeClassCreate(n);

  case DEREF_NODE:
    return n;

  case CAST_NODE:
    return analyzeCast(n);

  case EXPRST_NODE:
    return analyzeExprSt(n);

    /*
     * Should never happen 
     */
  case MAINVAR_NODE:
    return n;
  case FIELD_NODE:
    return n;
  case PARAM_NODE:
    return n;

    /*
     * Array nodes.
     */
  case ARRAY_NODE:
    return n;

  case ARRAYACCESS_NODE:
		return analyzeArrayAccess(n);

  case ARRAYCREATE_NODE:
		return analyzeArrayCreate(n);

	/* Accessing the length field of an array, this should
   * never get called. */
	case LENGTHACC_NODE:
		return n;
  }

  return n;
}
