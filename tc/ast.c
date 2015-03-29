/*
 * Ethan Burns Feb 8, 2006
 * ast.c implements functions for the AST nodes.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "ast.h"
#include "type.h"
#include "class.h"
#include "mtrack.h"
#include "error.h"

#define EASY_READ

/*
 * List of allocated memory for the AST. 
 */
static struct memassoc *memory = NULL;

/*
 * The root of the AST. 
 */
AstNode *ast = NULL;

/*
 * Sets up for building an AST. 
 */
void
initAst()
{
  memory = make_memassoc();
}

/*
 * Frees the memory associated with the AST. 
 */
void
freeAst()
{
  t_free_all(memory);
}

/*
 * Casts from any node type into an AstNode. 
 */
#define NODE(x) ((AstNode*)(x))

/*
 * Casts from any node type into an Expr. 
 */
#define EXPR(x) ((Expr*)(x))

/*
 * Initializes an AstNode structure. 
 */
static void
initAstNode(AstNode * n, NodeType t, unsigned int line)
{
  n->type = t;
  n->line = line;
}

/*
 * Initializes an Expr node. 
 */
static void
initExpr(Expr * e)
{
  e->type = NULL;
}

int
isExpr(AstNode * n)
{
  assert(n != NULL);

  return
      n->type == DEREF_NODE ||
      n->type == ARRAYCREATE_NODE ||
      n->type == CLASSCREATE_NODE ||
      n->type == EXPRID_NODE ||
      n->type == CAST_NODE ||
      n->type == UNARYOP_NODE ||
      n->type == BINARYOP_NODE ||
      n->type == INTLIT_NODE ||
      n->type == ARRAYACCESS_NODE ||
      n->type == METHODINVOC_NODE ||
      n->type == FIELDACCESS_NODE ||
      n->type == NULLLIT_NODE ||
      n->type == FORMALPARAM_NODE ||
      n->type == PARAM_NODE ||
      n->type == FIELD_NODE ||
      n->type == MAINVAR_NODE ||
      n->type == LENGTHACC_NODE ||
      n->type == THIS_NODE || n->type == SUPER_NODE;
}

AstNode *
makeExprSt(AstNode *expr)
{
  ExprSt *p;

  p = t_malloc(memory, sizeof(ExprSt));
  initAstNode(NODE(p), EXPRST_NODE, expr->line);

  p->expr = expr;

  return NODE(p);
}

/*
 * Creates an integer literal node with the given value. 
 */
AstNode *
makeIntLit(Literal * value)
{
  IntLit *p;

  p = t_malloc(memory, sizeof(IntLit));
  initAstNode(NODE(p), INTLIT_NODE, value->line);
  initExpr(EXPR(p));

  p->str = value->str;

  return NODE(p);
}

/*
 * Creates a null literal node. 
 */
AstNode *
makeNullLit(unsigned int line)
{
  Expr *p;

  p = t_malloc(memory, sizeof(Expr));
  initAstNode(NODE(p), NULLLIT_NODE, line);
  initExpr(EXPR(p));

  return NODE(p);
}

AstNode *
makeDeref(Type * type, AstNode * next, unsigned int line)
{
  Deref *p;

  p = t_malloc(memory, sizeof(Deref));
  initAstNode(NODE(p), DEREF_NODE, line);
  initExpr(EXPR(p));

  p->super.type = type;
  p->next = next;

  return NODE(p);
}

void
freeDeref(Deref * d)
{
  t_free(memory, d);
}

/*
 * Makes an array node. 
 */
AstNode *
makeArray(AstNode * child, unsigned int line)
{
  Array *p;

  p = t_malloc(memory, sizeof(Array));
  initAstNode(NODE(p), ARRAY_NODE, line);

  p->child = child;

  return NODE(p);
}

/*
 * Makes an integer type node. 
 */
AstNode *
makeInt(unsigned int line)
{
  AstNode *p;

  p = t_malloc(memory, sizeof(AstNode));
  initAstNode(p, INT_NODE, line);

  return p;
}

/*
 * Makes a dummy sequence with the given data and next node. 
 */
AstNode *
makeSeq(AstNode * data)
{
  extern unsigned int line;     /* Scan.l */
  Seq *p;

  p = t_malloc(memory, sizeof(Seq));
  initAstNode(NODE(p), SEQ_NODE, line);

  p->data = data;
  p->next = NULL;

  return NODE(p);
}

/*
 * Appends the first argument to the sequence given by the second
 * argument. The second argument is returned. 
 */
AstNode *
appendSeq(AstNode * child, AstNode * list)
{
  Seq *p;

  if (list == NULL) {
    printf("Error in appendSeq\n");
    exit(1);
  }

  for (p = ((Seq *) list); p->next != NULL; p = ((Seq *) p->next));
  p->next = child;

  return list;
}

/*
 * Makes an array access node. 
 */
AstNode *
makeArrayAccess(AstNode * name, AstNode * dexp, unsigned int line)
{
  ArrayAccess *p;

  p = t_malloc(memory, sizeof(ArrayAccess));
  initAstNode(NODE(p), ARRAYACCESS_NODE, line);
  initExpr(EXPR(p));

  p->name = name;
  p->dexp = dexp;

  return NODE(p);
}

/*
 * Makes a method invocation node. 
 */
AstNode *
makeMethodInvoc(AstNode * target, AstNode * ident, AstNode * args,
                unsigned int line)
{
  MethodInvoc *p;

  p = t_malloc(memory, sizeof(MethodInvoc));
  initAstNode(NODE(p), METHODINVOC_NODE, line);
  initExpr(EXPR(p));

  p->target = target;
  assert(ident->type == NAMEID_NODE);
  p->name = ((NameId *) ident)->name;
  p->args = args;
	p->method_type = NULL;

  return NODE(p);
}

/*
 * Makes a super (keyword) node. 
 */
AstNode *
makeSuper(unsigned int line)
{
  Expr *p;

  p = t_malloc(memory, sizeof(Expr));
  initAstNode(NODE(p), SUPER_NODE, line);
  initExpr(EXPR(p));

  return NODE(p);
}

/*
 * Makes a Field Access node. 
 */
AstNode *
makeFieldAccess(AstNode * target, AstNode * ident, unsigned int line)
{
  FieldAccess *p;

  p = t_malloc(memory, sizeof(FieldAccess));
  initAstNode(NODE(p), FIELDACCESS_NODE, line);
  initExpr(EXPR(p));

  p->target = target;
  assert(ident->type == NAMEID_NODE);
  p->name = ((NameId *) ident)->name;

  return NODE(p);
}

/*
 * Makes a this (keyword) node. 
 */
AstNode *
makeThis(unsigned int line)
{
  Expr *p;

  p = t_malloc(memory, sizeof(Expr));
  initAstNode(NODE(p), THIS_NODE, line);
  initExpr(EXPR(p));

  return NODE(p);
}

/*
 * Make an out statement node. 
 */
AstNode *
makeOutSt(AstNode * expr, unsigned int line)
{
  OutSt *p;

  p = t_malloc(memory, sizeof(OutSt));
  initAstNode(NODE(p), OUT_NODE, line);

  p->expr = expr;

  return NODE(p);
}

/*
 * Makes a delete node. 
 */
AstNode *
makeDeleteSt(AstNode * expr, unsigned int line)
{
  DeleteSt *p;

  p = t_malloc(memory, sizeof(DeleteSt));
  initAstNode(NODE(p), DELETE_NODE, line);

  p->expr = expr;

  return NODE(p);
}

/*
 * Makes a return node. 
 */
AstNode *
makeReturnSt(AstNode * expr, unsigned int line)
{
  ReturnSt *p;

  p = t_malloc(memory, sizeof(ReturnSt));
  initAstNode(NODE(p), RETURN_NODE, line);

  p->expr = expr;

  return NODE(p);
}

/*
 * Makes a while statement node. 
 */
AstNode *
makeWhileSt(AstNode * expr, AstNode * stats, unsigned int line)
{
  WhileSt *p;

  p = t_malloc(memory, sizeof(WhileSt));
  initAstNode(NODE(p), WHILE_NODE, line);

  p->expr = expr;
  p->stats = stats;

  return NODE(p);
}

/*
 * Makes an array creation expression node. 
 */
AstNode *
makeArrayCreate(AstNode * type, AstNode * expr, unsigned int dims,
                unsigned int line)
{
  ArrayCreate *p;

  p = t_malloc(memory, sizeof(ArrayCreate));
  initAstNode(NODE(p), ARRAYCREATE_NODE, line);
  initExpr(EXPR(p));

  p->type = type;
  p->expr = expr;
  p->dims = dims;

  return NODE(p);
}

/*
 * Makes a class instance creation node. 
 */
AstNode *
makeClassCreate(AstNode * type, AstNode * args, unsigned int line)
{
  ClassCreate *p;

  p = t_malloc(memory, sizeof(ClassCreate));
  initAstNode(NODE(p), CLASSCREATE_NODE, line);
  initExpr(EXPR(p));

  p->type = type;
  p->args = args;
	p->cons = NULL;

  return NODE(p);
}

/*
 * Makes an expression identifier node. 
 */
AstNode *
makeExprId(Literal * name)
{
  ExprId *p;

  p = t_malloc(memory, sizeof(ExprId));
  initAstNode(NODE(p), EXPRID_NODE, name->line);
  initExpr(EXPR(p));

  p->name = name->str;

  return NODE(p);
}

/*
 * Makes a name identifier node. 
 */
AstNode *
makeNameId(Literal * name)
{
  NameId *p;

  p = t_malloc(memory, sizeof(NameId));
  initAstNode(NODE(p), NAMEID_NODE, name->line);

  p->name = name->str;

  return NODE(p);
}

/*
 * Makes a cast expression node. 
 */
AstNode *
makeCast(AstNode * type, AstNode * expr, unsigned int line)
{
  Cast *p;

  p = t_malloc(memory, sizeof(Cast));
  initAstNode(NODE(p), CAST_NODE, line);
  initExpr(EXPR(p));

  p->type = type;
  p->expr = expr;

  return NODE(p);
}

/*
 * Makes a unary operation expression of the given type. 
 */
AstNode *
makeUnaryOp(OpType op, AstNode * left, unsigned int line)
{
  UnaryOp *p;

  p = t_malloc(memory, sizeof(UnaryOp));
  initAstNode(NODE(p), UNARYOP_NODE, line);
  initExpr(EXPR(p));

  p->op = op;
  p->left = left;

  return NODE(p);
}

/*
 * Makes a binary operation node with the given operation. 
 */
AstNode *
makeBinaryOp(OpType op, AstNode * left, AstNode * right, unsigned int line)
{
  BinaryOp *p;

  p = t_malloc(memory, sizeof(BinaryOp));
  initAstNode(NODE(p), BINARYOP_NODE, line);
  initExpr(EXPR(p));

  p->op = op;
  p->left = left;
  p->right = right;

  return NODE(p);
}

/*
 * Makes an if then else node. 
 */
AstNode *
makeIfElseSt(AstNode * expr, AstNode * tstat, AstNode * fstat,
             unsigned int line)
{
  IfElseSt *p;

  p = t_malloc(memory, sizeof(IfElseSt));
  initAstNode(NODE(p), IFELSE_NODE, line);

  p->expr = expr;
  p->tstat = tstat;
  p->fstat = fstat;

  return NODE(p);
}

/*
 * Makes a main variable declaration node. 
 */
AstNode *
makeMainVarDecl(AstNode * type, AstNode * decl, unsigned int line)
{
  VarDecl *p;

  p = t_malloc(memory, sizeof(VarDecl));
  initAstNode(NODE(p), MAINVARDECL_NODE, line);

  p->type = type;
  p->decl = decl;
  p->dims = NULL;

  return NODE(p);
}

/*
 * Makes a formal parameter node. 
 */
AstNode *
makeFormalParam(AstNode * type, AstNode * decl, unsigned int line)
{
  FormalParam *p;

  p = t_malloc(memory, sizeof(FormalParam));
  initAstNode(NODE(p), FORMALPARAM_NODE, line);
  initExpr(EXPR(p));

  p->type = type;
  p->decl = decl;
  p->num = 0;
  p->name = NULL;

  return NODE(p);
}

/*
 * Make a destructor node. 
 */
AstNode *
makeDestructor(AstNode * ident, AstNode * body, unsigned int line)
{
  Destructor *p;

  p = t_malloc(memory, sizeof(Destructor));
  initAstNode(NODE(p), DESTRUCTOR_NODE, line);

  assert(ident->type == NAMEID_NODE);
  p->name = ((NameId *) ident)->name;
  p->body = body;

  return NODE(p);
}

/*
 * Makes a constructor invocation node. 
 */
AstNode *
makeConstInvoc(AstNode * target, AstNode * args, unsigned int line)
{
  ConstInvoc *p;

  p = t_malloc(memory, sizeof(ConstInvoc));
  initAstNode(NODE(p), CONSTINVOC_NODE, line);
  initExpr(EXPR(p));

  p->target = target;
  p->args = args;
  p->cons = NULL;

  return NODE(p);
}

/*
 * Makes a constructor declarator node. 
 */
AstNode *
makeConstDeclator(AstNode * ident, AstNode * param, unsigned int line)
{
  MethDeclator *p;

  p = t_malloc(memory, sizeof(MethDeclator));
  initAstNode(NODE(p), CONSTDECLATOR_NODE, line);

  assert(ident->type == NAMEID_NODE);
  p->name = ((NameId *) ident)->name;
  p->param = param;

  return NODE(p);
}

/*
 * Makes a constructor declaration node. 
 */
AstNode *
makeConstDecl(AstNode * decl, AstNode * body, unsigned int line)
{
  MemberDecl *p;

  p = t_malloc(memory, sizeof(MemberDecl));
  initAstNode(NODE(p), CONSTDECL_NODE, line);

  p->type = NULL;
  p->decl = decl;
  p->body = body;
  p->t = NULL;
	p->mtype = NULL;

  return NODE(p);
}

/*
 * Makes a method declarator node. 
 */
AstNode *
makeMethDeclator(AstNode * ident, AstNode * param, unsigned int line)
{
  MethDeclator *p;

  p = t_malloc(memory, sizeof(MethDeclator));
  initAstNode(NODE(p), METHDECLATOR_NODE, line);

  assert(ident->type == NAMEID_NODE);
  p->name = ((NameId *) ident)->name;
  p->param = param;

  return NODE(p);
}

/*
 * Makes a method declaration node. 
 */
AstNode *
makeMethDecl(AstNode * type, AstNode * decl, AstNode * body,
             unsigned int line)
{
  MemberDecl *p;

  p = t_malloc(memory, sizeof(MemberDecl));
  initAstNode(NODE(p), METHDECL_NODE, line);

  p->type = type;
  p->decl = decl;
  p->body = body;
  p->t = NULL;
	p->mtype = NULL;

  return NODE(p);
}

/*
 * Makes a field declaration node. 
 */
AstNode *
makeFieldDecl(AstNode * type, AstNode * decl, unsigned int line)
{
  MemberDecl *p;

  p = t_malloc(memory, sizeof(MemberDecl));
  initAstNode(NODE(p), FIELDDECL_NODE, line);

  p->type = type;
  p->decl = decl;
  p->body = NULL;
  p->t = NULL;
	p->mtype = NULL;

  return NODE(p);
}

/*
 * Makes a class declaration node. 
 */
AstNode *
makeClass(AstNode * ident, AstNode * parent, AstNode * body,
          unsigned int line)
{
  NameId *nameid;
  Class *p;

  p = t_malloc(memory, sizeof(Class));
  initAstNode(NODE(p), CLASS_NODE, line);

  assert(ident->type == NAMEID_NODE);
  nameid = (NameId *) ident;
  p->name = nameid->name;

  if (parent != NULL) {
    assert(parent->type == NAMEID_NODE);
    nameid = (NameId *) parent;
    p->pname = nameid->name;
  } else {
    p->pname = NULL;
  }

  p->body = body;

  return NODE(p);
}

/*
 * Makes a main function declaration node. 
 */
AstNode *
makeMainFunc(AstNode * body, unsigned int line)
{
  MainFunc *p;

  p = t_malloc(memory, sizeof(MainFunc));
  initAstNode(NODE(p), MAINFUNC_NODE, line);

  p->body = body;

  return NODE(p);
}

/*
 * Makes a compilation unit node. 
 */
AstNode *
makeCUnit(AstNode * list)
{
  ast = list;

  return list;
}

/*
 * Returns a Type* given an AstNode*. 
 */
Type *
buildType(AstNode * n)
{
  NameId *ident;
  Type *retval = NULL;

  /*
   * n can either be an INT_NODE, an ARRAY_NODE, or a NAMEID_NODE. 
   */
  if (n->type == INT_NODE) {
    retval = makeIntType();

  } else if (n->type == ARRAY_NODE) {
    retval = (Type *) makeArrayType(buildType(((Array *) n)->child));

  } else {
    assert(n->type == NAMEID_NODE);

    ident = (NameId *) n;

    retval = (Type *) getClass(ident->name);

    if (retval == NULL) {
      error("class name expected", &ident->super);
      retval = makeErrorType();
    }

  }

  return retval;
}

/*
 * Returns a list of arrays given a declarator. 
 */
Type *
getDims(AstNode * n)
{
  if (n == NULL)
    return NULL;

  switch (n->type) {
  case ARRAY_NODE:
    return (Type *) makeArrayType(getDims(((Array *) n)->child));
  default:
    return NULL;
  }
}

/*
 * Returns the identifier given a declarator. 
 */
char *
getIdent(AstNode * n)
{
  AstNode *p;

  for (p = n; p != NULL && p->type == ARRAY_NODE;
       p = ((Array *) p)->child);
  assert(p != NULL);
  assert(p->type == NAMEID_NODE);

  return ((NameId *) p)->name;
}

AstNode *
makeObject()
{
  AstNode *object;
  AstNode *equMeth,
  *construct,
  *destruct;
  AstNode *declator,
  *fparam;
  AstNode *ret,
  *eq;
  Literal *classLit,
  *oLit,
  *equalsLit;

  /*
   * Build the 
   * int equals(Object o) { return this == o; } 
   * method.
   */
  classLit = makeLiteral(OBJECT_NAME, 0);
  oLit = makeLiteral("o", 0);
  fparam = makeFormalParam(makeNameId(classLit), makeNameId(oLit), 0);
  equalsLit = makeLiteral("equals", 0);
  declator = makeMethDeclator(makeNameId(equalsLit), makeSeq(fparam), 0);
  eq = makeBinaryOp(EQU_OP, makeThis(0), makeExprId(oLit), 0);
  ret = makeReturnSt(eq, 0);
  equMeth = makeMethDecl(makeInt(0), declator, makeSeq(ret), 0);

  /*
   * Build
   * Object() { return; }
   * constructor
   */
  declator = makeConstDeclator(makeNameId(classLit), NULL, 0);
  construct = makeConstDecl(declator, makeSeq(makeReturnSt(NULL, 0)), 0);

  /*
   * Build
   * ~Object() { return; }
   * destructor
   */
  destruct = makeDestructor(makeNameId(classLit),
                            makeSeq(makeReturnSt(NULL, 0)), 0);

  /*
   * Link the class all together.
   */
  object = makeClass(makeNameId(classLit), NULL,
                     appendSeq(appendSeq(makeSeq(equMeth),
                                         makeSeq(construct)),
                               makeSeq(destruct)), 0);

  return object;
}

AstNode *
makeDefaultConstructor(MethodType * cons)
{
  NameId *nameid;
  MethDeclator *mdeclator;
  ConstInvoc *cinvoc;
  MemberDecl *mdecl;
  Literal *lit;

  lit = makeLiteral(cons->name, 0);
  nameid = (NameId *) makeNameId(lit);
  mdeclator = (MethDeclator *) makeConstDeclator(&nameid->super, NULL, 0);
  cinvoc = (ConstInvoc *) makeConstInvoc(makeSuper(0), NULL, 0);
  mdecl = (MemberDecl *) makeConstDecl(&mdeclator->super, 
				       makeSeq(&cinvoc->super.super), 0);
  mdecl->t = &cons->super;

  return makeSeq(&mdecl->super);
}

AstNode *
makeDefaultDestructor(ClassType * class_type)
{
  AstNode *dest;

  dest = makeDestructor(makeNameId(makeLiteral(class_type->name, 0)),
                        makeSeq(makeReturnSt(NULL, 0)), 0);

  return makeSeq(dest);
}

static void
printArrayCreate(AstNode * n, unsigned int seqnum)
{
  ArrayCreate *arrayc;

  assert(n->type == ARRAYCREATE_NODE);
  arrayc = (ArrayCreate *) n;

  printf("ArrayCreationExpression (%d)\n", arrayc->dims);
  _printNode(arrayc->type, seqnum);
  _printNode(arrayc->expr, seqnum);
}

static void
printClassCreate(AstNode * n, unsigned int seqnum)
{
  ClassCreate *classc;

  assert(n->type == CLASSCREATE_NODE);
  classc = (ClassCreate *) n;

  printf("ClassCreationExpression\n");
  _printNode(classc->args, seqnum);
}

static void
printExprId(AstNode * n)
{
  ExprId *exprid;

  assert(n->type == EXPRID_NODE);
  exprid = (ExprId *) n;

  printf("ExpressionIdentifier (%s)\n", exprid->name);
}

static void
printMainVar(AstNode * n)
{
  ExprId *exprid;

  assert(n->type == MAINVAR_NODE);
  exprid = (ExprId *) n;

  printf("MainVariable (%s)\n", exprid->name);
}

static void
printParam(AstNode * n)
{
  ExprId *exprid;

  assert(n->type == PARAM_NODE);
  exprid = (ExprId *) n;

  printf("Parameter (%s)\n", exprid->name);
}

static void
printField(AstNode * n)
{
  ExprId *exprid;

  assert(n->type == FIELD_NODE);
  exprid = (ExprId *) n;

  printf("Field (%s)\n", exprid->name);
}

static void
printCast(AstNode * n, unsigned int seqnum)
{
  Cast *cast;

  assert(n->type == CAST_NODE);
  cast = (Cast *) n;

  printf("CastExpression\n");
  _printNode(cast->expr, seqnum);
}

static void
printUnaryOp(AstNode * n, unsigned int seqnum)
{
  UnaryOp *unaryop;

  assert(n->type == UNARYOP_NODE);
  unaryop = (UnaryOp *) n;

  switch (unaryop->op) {
  case NEG_OP:
    printf("-\n");
    break;
  case NOT_OP:
    printf("!\n");
    break;

    /*
     * This should *NEVER* happen. 
     */
  default:
    printf("ERROR UNARYOP_NODE\n");
    exit(1);

  }

  _printNode(unaryop->left, seqnum);
}

static void
printBinaryOp(AstNode * n, unsigned int seqnum)
{
  BinaryOp *binaryop;

  assert(n->type == BINARYOP_NODE);
  binaryop = (BinaryOp *) n;

  switch (binaryop->op) {
  case ASI_OP:
    printf("=\n");
    break;
  case EQU_OP:
    printf("==\n");
    break;
  case LES_OP:
    printf("<\n");
    break;
  case GRT_OP:
    printf(">\n");
    break;
  case ADD_OP:
    printf("+\n");
    break;
  case SUB_OP:
    printf("-\n");
    break;
  case MUL_OP:
    printf("*\n");
    break;
  case DIV_OP:
    printf("/\n");
    break;

    /*
     * This should *NEVER* happen. 
     */
  default:
    printf("ERROR BINARYOP_NODE\n");
    exit(1);
  }

  _printNode(binaryop->left, seqnum);
  _printNode(binaryop->right, seqnum);
}

static void
printSeq(AstNode * n, unsigned int seqnum)
{
  unsigned int i;
  Seq *seq;

  assert(n->type == SEQ_NODE);
  seq = (Seq *) n;

  printf("SEQ(%d)----------------------\n", seqnum);
  _printNode(seq->data, seqnum + 1);

  if (seq->next == NULL) {
#ifdef EASY_READ
    for (i = 0; i < seqnum; i++)
      printf(" ");
#endif                          /* EASY_READ */
    printf("NULL(%d)---------------------\n", seqnum);
  } else {
    _printNode(seq->next, seqnum);
  }
}

static void
printIntLit(AstNode * n)
{
  IntLit *intlit;

  assert(n->type == INTLIT_NODE);
  intlit = (IntLit *) n;

  if (intlit->str != NULL)
    printf("IntegerLiteral (%s)\n", intlit->str);
}

static void
printArray(AstNode * n, unsigned int seqnum)
{
  Array *array;

  assert(n->type == ARRAY_NODE);
  array = (Array *) n;

  printf("Array\n");
  _printNode(array->child, seqnum);
}

static void
printArrayAccess(AstNode * n, unsigned int seqnum)
{
  ArrayAccess *arraya;

  assert(n->type == ARRAYACCESS_NODE);
  arraya = (ArrayAccess *) n;

  printf("ArrayAccess\n");
  _printNode(arraya->name, seqnum);
  _printNode(arraya->dexp, seqnum);
}

static void
printMethodInvoc(AstNode * n, unsigned int seqnum)
{
  MethodInvoc *minvoc;

  assert(n->type == METHODINVOC_NODE);
  minvoc = (MethodInvoc *) n;

  printf("MethodInvocation (%s)\n", minvoc->name);
  _printNode(minvoc->target, seqnum);
  _printNode(minvoc->args, seqnum);
}

static void
printFieldAccess(AstNode * n, unsigned int seqnum)
{
  FieldAccess *facc;

  assert(n->type == FIELDACCESS_NODE);
  facc = (FieldAccess *) n;

  printf("FieldAccess (%s)\n", facc->name);
  _printNode(facc->target, seqnum);
}

static void
printWhile(AstNode * n, unsigned int seqnum)
{
  WhileSt *wstat;

  assert(n->type == WHILE_NODE);
  wstat = (WhileSt *) n;

  printf("While\n");
  _printNode(wstat->expr, seqnum);
  _printNode(wstat->stats, seqnum);
}

static void
printIfElse(AstNode * n, unsigned int seqnum)
{
  IfElseSt *ifelse;

  assert(n->type == IFELSE_NODE);
  ifelse = (IfElseSt *) n;

  printf("IfThenElse\n");
  _printNode(ifelse->expr, seqnum);
  _printNode(ifelse->tstat, seqnum);
  _printNode(ifelse->fstat, seqnum);
}

static void
printMainVarDecl(AstNode * n, unsigned int seqnum)
{
  VarDecl *var;

  assert(n->type == MAINVARDECL_NODE);
  var = (VarDecl *) n;

  printf("MainVariableDeclaration\n");
  _printNode(var->type, seqnum);
  _printNode(var->decl, seqnum);
}

static void
printFormalParm(AstNode * n)
{
  FormalParam *fparm;

  assert(n->type == FORMALPARAM_NODE);
  fparm = (FormalParam *) n;

  printf("FormalParameter%d (%s)\n", fparm->num, fparm->name);
}

static void
printDestructor(AstNode * n, unsigned int seqnum)
{
  Destructor *destr;

  assert(n->type == DESTRUCTOR_NODE);
  destr = (Destructor *) n;

  printf("Destructor (%s)\n", destr->name);
  _printNode(destr->body, seqnum);
}

static void
printConstInvoc(AstNode * n, unsigned int seqnum)
{
  ConstInvoc *cinvoc;

  assert(n->type == CONSTINVOC_NODE);
  cinvoc = (ConstInvoc *) n;

  printf("ConstructorInvocation\n");
  _printNode(cinvoc->target, seqnum);
  _printNode(cinvoc->args, seqnum);
}

static void
printConstDeclor(AstNode * n, unsigned int seqnum)
{
  MethDeclator *cdecl;

  assert(n->type == CONSTDECLATOR_NODE);
  cdecl = (MethDeclator *) n;

  printf("ConstructorDeclarator (%s)\n", cdecl->name);
  _printNode(cdecl->param, seqnum);
}

static void
printMethDeclor(AstNode * n, unsigned int seqnum)
{
  MethDeclator *mdecl;

  assert(n->type == METHDECLATOR_NODE);
  mdecl = (MethDeclator *) n;

  printf("MethodDeclarator (%s)\n", mdecl->name);
  _printNode(mdecl->param, seqnum);
}

static void
printMethDecl(AstNode * n, unsigned int seqnum)
{
  MemberDecl *mdecl;

  assert(n->type == METHDECL_NODE);
  mdecl = (MemberDecl *) n;

  printf("MethodDeclaration\n");
  _printNode(mdecl->type, seqnum);
  _printNode(mdecl->decl, seqnum);
  _printNode(mdecl->body, seqnum);
}

static void
printConstDecl(AstNode * n, unsigned int seqnum)
{
  MemberDecl *cdecl;

  assert(n->type == CONSTDECL_NODE);
  cdecl = (MemberDecl *) n;

  printf("ConstructorDeclaration\n");
  _printNode(cdecl->decl, seqnum);
  _printNode(cdecl->body, seqnum);
}

static void
printFieldDecl(AstNode * n, unsigned int seqnum)
{
  MemberDecl *fdecl;

  assert(n->type == FIELDDECL_NODE);
  fdecl = (MemberDecl *) n;

  printf("FieldDeclaration\n");
  _printNode(fdecl->type, seqnum);
  _printNode(fdecl->decl, seqnum);
}

static void
printClass(AstNode * n, unsigned int seqnum)
{
  Class *class;

  assert(n->type == CLASS_NODE);
  class = (Class *) n;

  if (class->pname == NULL)
    printf("ClassDeclaration (%s)\n", class->name);
  else
    printf("ClassDeclaration (%s extends %s)\n", class->name,
           class->pname);

  _printNode(class->body, seqnum);
}

/*
 * Prints an AstNode. 
 */
void
_printNode(AstNode * n, unsigned int seqnum)
{
  unsigned int i;

#ifdef EASY_READ
  for (i = 0; i < seqnum; i++)
    printf(" ");
#endif                          /* EASY_READ */

  if (n == NULL) {
    printf("NULL\n");
    return;
  }

  if (n->type != SEQ_NODE)
    printf("line %d: ", n->line);

  if (isExpr(n) == 1) {
    printf("{");
    printType(((Expr *) n)->type);
    printf("} ");
  }

  switch (n->type) {
  case NULLLIT_NODE:
    printf("NullLiteral\n");
    break;
  case INT_NODE:
    printf("Integer\n");
    break;
  case THIS_NODE:
    printf("This\n");
    break;
  case SUPER_NODE:
    printf("Super\n");
    break;

  case ARRAYCREATE_NODE:
    printArrayCreate(n, seqnum);
    break;

  case CLASSCREATE_NODE:
    printClassCreate(n, seqnum);
    break;

  case MAINVAR_NODE:
    printMainVar(n);
    break;

  case FIELD_NODE:
    printField(n);
    break;

  case PARAM_NODE:
    printParam(n);
    break;

  case EXPRID_NODE:
    printExprId(n);
    break;

  case CAST_NODE:
    printCast(n, seqnum);
    break;

  case UNARYOP_NODE:
    printUnaryOp(n, seqnum);
    break;

  case BINARYOP_NODE:
    printBinaryOp(n, seqnum);
    break;

  case SEQ_NODE:
    printSeq(n, seqnum);
    break;

  case INTLIT_NODE:
    printIntLit(n);
    break;

  case ARRAY_NODE:
    printArray(n, seqnum);
    break;

  case NAMEID_NODE:
    printf("NameIdentifier (%s)\n", ((NameId *) n)->name);
    break;

  case ARRAYACCESS_NODE:
    printArrayAccess(n, seqnum);
    break;

  case METHODINVOC_NODE:
    printMethodInvoc(n, seqnum);
    break;

	case LENGTHACC_NODE:
  case FIELDACCESS_NODE:
    printFieldAccess(n, seqnum);
    break;

  case OUT_NODE:
    printf("Out\n");
    _printNode(((OutSt *) n)->expr, seqnum);
    break;

  case DELETE_NODE:
    printf("Delete\n");
    _printNode(((DeleteSt *) n)->expr, seqnum);
    break;

  case RETURN_NODE:
    printf("Return\n");
    _printNode(((ReturnSt *) n)->expr, seqnum);
    break;

  case WHILE_NODE:
    printWhile(n, seqnum);
    break;

  case IFELSE_NODE:
    printIfElse(n, seqnum);
    break;

  case MAINVARDECL_NODE:
    printMainVarDecl(n, seqnum);
    break;

  case FORMALPARAM_NODE:
    printFormalParm(n);
    break;

  case DESTRUCTOR_NODE:
    printDestructor(n, seqnum);
    break;

  case CONSTINVOC_NODE:
    printConstInvoc(n, seqnum);
    break;

  case CONSTDECLATOR_NODE:
    printConstDeclor(n, seqnum);
    break;

  case METHDECLATOR_NODE:
    printMethDeclor(n, seqnum);
    break;

  case METHDECL_NODE:
    printMethDecl(n, seqnum);
    break;

  case CONSTDECL_NODE:
    printConstDecl(n, seqnum);
    break;

  case FIELDDECL_NODE:
    printFieldDecl(n, seqnum);
    break;

  case CLASS_NODE:
    printClass(n, seqnum);
    break;

  case MAINFUNC_NODE:
    printf("MainFunctionDeclaration\n");
    _printNode(((MainFunc *) n)->body, seqnum);
    break;

  case DEREF_NODE:
    printf("Deref\n");
    _printNode(((Deref *) n)->next, seqnum);
    break;

	case EXPRST_NODE:
		printf("ExprSt\n");
		_printNode( ((ExprSt*)n)->expr, seqnum);

  }

}
