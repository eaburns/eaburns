/*
 * gencode.c Function definitions for the code generation AST traversal.
 * Ethan Burns -- Created Wed Mar 29 14:18:54 2006
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "codegen.h"
#include "class.h"
#include "type.h"

/* The code output file. */
static FILE *code_file = NULL;

/* Number of the next label to use. */
static unsigned int label_num = 0;

/* Set to a 1 if the code being generated is in the main function body, else
 * set to 0. */
static int main_body = 0;

/* Set to a 1 if generating code for a constructor or destructor body, else set to 0. */
static int constdest= 0;

#define ECHO_CODE
#if defined(ECHO_CODE)
#define print_code(fmt...)          \
	do {                        \
  	fprintf(code_file, fmt);    \
  	fprintf(stdout, fmt);       \
	} while(0)

#define comment_code(msg...) \
  fprintf(stdout, "\n/* " msg " */\n")

#else

#define print_code(fmt...) \
  	fprintf(code_file, fmt)

#define comment_code(...)

#endif

static void gencode(AstNode *root);

/* 10 is max digits in the longest unsigned int value, add a . an L and a \0
 * and we have a maximum of 13 characters in a label name, add 1 to make it
 * an even (non-unlucky) number and we get 14. */
#define MAX_LABEL_LEN 14

/* Generates a label in the form .L## where ## is a number from 0-(2^32).
 * Generated labels must be freed with free(). */
static char *
genlabel()
{
  char *label;

  label = malloc(sizeof(char)*MAX_LABEL_LEN); 
  if(label == NULL) {
    fprintf(stderr, "genlabel: malloc out of memory\n");
    exit(1);
  }

  snprintf(label, MAX_LABEL_LEN, ".L%u", label_num++);

  return label;
}

static void
genMainFunc(AstNode *root)
{
  MainFunc *mainfun;

  assert(root->type == MAINFUNC_NODE);
  mainfun = (MainFunc*)root;

  comment_code("Main Function Begin"); 
  print_code("\t.align 4\n"
	     "\t.globl main\n"
	     "main:\n"
	     "\tpushl %%ebp\n"
	     "\tmovl %%esp, %%ebp\n");

  main_body = 1;
  gencode(mainfun->body);
  main_body = 0;

  comment_code("Main Function End"); 
  print_code("main$exit:\n"
	     "\tpopl %%ebp\n"
	     "\tret\n");

}

static void
genSeq(AstNode *root)
{
  Seq *seq;

  assert(root->type == SEQ_NODE);
  seq = (Seq*)root;

  gencode(seq->data);
  gencode(seq->next);
}

static void
genMainVarDecl(AstNode *root)
{
  VarDecl *var;
  Seq *seq;

  assert(root->type == MAINVARDECL_NODE);
  var = (VarDecl*)root;

  /* var->type and var->dims don't matter for generating code. */

  /* Variable declarators are sequences of identifiers or arrays followed
   * by identifiers. */
  assert(var->decl->type == SEQ_NODE);
  comment_code("Variable Declarators");
  print_code("\t.data\n");
  for(seq = (Seq*)var->decl; seq != NULL; seq = (Seq*)seq->next) {
    print_code("mainvar$%s: .long 0\n", getIdent(seq->data));
  }
  print_code("\t.text\n");
}

static void
genMainVar(AstNode *root)
{
  ExprId *mainvar;

  assert(root->type == MAINVAR_NODE);
  mainvar = (ExprId*)root;

  comment_code("Main Variable Useage");
  print_code("\tpushl $mainvar$%s\n", mainvar->name);	
}

static void
genIntLit(AstNode *root)
{
  IntLit *intlit;

  assert(root->type == INTLIT_NODE);
  intlit = (IntLit*)root;

  comment_code("Integer Literal");
  print_code("\tpushl $%s\n", intlit->str);
}

static void
genNullLit(AstNode *root)
{
  assert(root->type == NULLLIT_NODE);
  comment_code("Null Literal");
  print_code("\tpushl $0\n");
}

static void
genExprSt(AstNode *root)
{
  ExprSt *exprst;

  assert(root->type == EXPRST_NODE);
  exprst = (ExprSt*)root;

  gencode(exprst->expr);

  /* Remove expression result from the stack. */
  comment_code("Expression Statement Cleanup");
  print_code("\taddl $4, %%esp\n");
}

static void
genBinaryOp(AstNode *root)
{
  BinaryOp *binop;

  assert(root->type == BINARYOP_NODE);
  binop = (BinaryOp*)root;

  gencode(binop->left);
  gencode(binop->right);

  switch(binop->op) {
  case ASI_OP:
    comment_code("Binary Operation: Assignment");
    print_code("\tpopl %%eax\n"
	       "\tpopl %%edx\n"
               "\tmovl %%eax, (%%edx)\n"
               "\tpushl %%eax\n");
    break;

  case ADD_OP:
    comment_code("Binary Operation: Addition");
    print_code("\tpopl %%edx\n"
	       "\tpopl %%eax\n"
               "\taddl %%edx, %%eax\n"
               "\tpushl %%eax\n");
    break;

  case SUB_OP:
    comment_code("Binary Operation: Subtraction");
    print_code("\tpopl %%edx\n"
	       "\tpopl %%eax\n"
               "\tsubl %%edx, %%eax\n"
               "\tpushl %%eax\n");
    break;

  case MUL_OP:
    comment_code("Binary Operation: Multiply");
    print_code("\tpopl %%edx\n"
	       "\tpopl %%eax\n"
               "\timull %%edx, %%eax\n"
               "\tpushl %%eax\n");
    break;

  case DIV_OP:
    comment_code("Binary Operation: Division");
    print_code("\tpushl $%u\n"
	       "\tcall RTS_checkForZeroDivide\n"
               "\taddl $4, %%esp\n"
               "\tpopl %%ecx\n"
               "\tpopl %%eax\n"
               "\tcltd\n"
               "\tidivl %%ecx, %%eax\n"
               "\tpushl %%eax\n", binop->super.super.line);
		break;
  case EQU_OP:
    comment_code("Binary Operation: Equal");
    print_code("\tpopl %%eax\n"
               "\tpopl %%edx\n"
               "\tcmpl %%eax, %%edx\n"
               "\tsete %%al\n"
               "\tmovzbl %%al, %%eax\n"
               "\tpushl %%eax\n");
    break;
  case LES_OP:
    comment_code("Binary Operation: Less Than");
    print_code("\tpopl %%eax\n"
               "\tpopl %%edx\n"
               "\tcmpl %%eax, %%edx\n"
               "\tsetl %%al\n"
               "\tmovzbl %%al, %%eax\n"
               "\tpushl %%eax\n");
    break;
  case GRT_OP:
    comment_code("Binary Operation: Greater Than");
    print_code("\tpopl %%eax\n"
               "\tpopl %%edx\n"
               "\tcmpl %%eax, %%edx\n"
               "\tsetg %%al\n"
               "\tmovzbl %%al, %%eax\n"
               "\tpushl %%eax\n");
    break;

  case NEG_OP: /* FALL THROUGH */
  case NOT_OP:
    assert("Not Possible" == 0);
  }
}

static void
genUnaryOp(AstNode *root)
{
  UnaryOp *unary;

  assert(root->type == UNARYOP_NODE);
  unary = (UnaryOp*)root;

  gencode(unary->left);

  switch(unary->op) {
  case NEG_OP:
    comment_code("Unary Operation: Negation");
    print_code("\tpopl %%eax\n"
               "\tnegl %%eax\n"
               "\tpushl %%eax\n");
    break;

  case NOT_OP:
    comment_code("Unary Operation: Logical Negation");
    print_code("\tpopl %%eax\n"
               "\tcmpl $0, %%eax\n"
               "\tsete %%al\n"
               "\tmovzbl %%al, %%eax\n"
               "\tpushl %%eax\n");
    break;

  default:
    assert("Not Possible" == 0);
  }
}

static void
genDeref(AstNode *root)
{
  Deref *deref;

  assert(root->type == DEREF_NODE);
  deref = (Deref*)root;

  gencode(deref->next);		

  comment_code("Deref");
  print_code("\tpopl %%eax\n"
	     "\tmovl (%%eax), %%eax\n"
             "\tpushl %%eax\n");
}

static void
genOut(AstNode *root)
{
  OutSt *out;

  assert(root->type == OUT_NODE);
  out = (OutSt*)root;

  gencode(out->expr);	

  comment_code("Out");
  print_code("\tcall RTS_outputInteger\n"
             "\taddl $4, %%esp\n");
}

static void
genReturn(AstNode *root)
{
  ReturnSt *ret;
  MethodType *method_type;
  ClassType *class_type;
  char *name;

  assert(root->type == RETURN_NODE);
  ret = (ReturnSt*)root;

  gencode(ret->expr);

  comment_code("Return");

  if(main_body == 1) {
    print_code("\tpopl %%eax\n"
               "\tjmp main$exit\n");
  } else if(constdest == 1) {
    class_type = getCurClass();
    assert(class_type != NULL);

    method_type = getCurMethod();

    if(method_type == NULL) {
      name = getDestructorName(class_type->name);
    } else {
      name = getMungedName(method_type);
    }
	
    print_code("\tjmp %s$exit\n", name);

  } else {
    class_type = getCurClass();
    assert(class_type != NULL);
    method_type = getCurMethod();
    assert(method_type != NULL);

    name = getMungedName(method_type);	

    print_code("\tpopl %%eax\n"
               "\tjmp %s$exit\n", name);

    free(name);
  }
}

static void
genIfElse(AstNode *root)
{
  IfElseSt *ifelse;
  char *lfalse, *lend;

  assert(root->type == IFELSE_NODE);
  ifelse = (IfElseSt*)root;

  lfalse =  genlabel();
  lend =  genlabel();

  comment_code("If");
  gencode(ifelse->expr);
  print_code("\tpopl %%eax\n"
             "\tcmpl $0, %%eax\n"
             "\tje %s\n", lfalse);

  comment_code("Then");
  gencode(ifelse->tstat);
  print_code("\tjmp %s\n", lend);

  comment_code("Else");
  print_code("%s:\n", lfalse);
  gencode(ifelse->fstat);

  print_code("%s:\n", lend);

  free(lend);
  free(lfalse);
}

static void
genWhile(AstNode *root)
{
  WhileSt *wh;
  char *lbegin, *lend;

  assert(root->type == WHILE_NODE);
  wh = (WhileSt*)root;
	
  lbegin = genlabel();	
  lend = genlabel();	

  comment_code("While");
  print_code("%s:\n", lbegin);
  gencode(wh->expr);
  print_code("\tpopl %%eax\n"
             "\tcmpl $0, %%eax\n"
             "\tje %s\n", lend);
  gencode(wh->stats);
  print_code("\tjmp %s\n"
             "%s:\n", lbegin, lend);

  free(lend);
  free(lbegin);
}

static void
genClass(AstNode *root)
{
  Class *class;
  ClassType *class_type;

  assert(root->type == CLASS_NODE);
  class = (Class*)root;

  class_type = getClass(class->name);
  assert(class_type != NULL);

  setCurrentClass(class_type);
  fprintf(code_file, "\n/* <------%s------> */\n", class_type->name);
  gencode(class->body);
  setCurrentClass(NULL);
}

static void
genMethDecl(AstNode *root)
{
  MemberDecl *mdecl;
  MethodType *method_type;
  char *name;

  assert(root->type == METHDECL_NODE);
  mdecl = (MemberDecl*)root;
  assert(mdecl->mtype != NULL);
  assert(mdecl->mtype->super.tag == METHOD_TYPE);
  method_type = mdecl->mtype;

  name = getMungedName(method_type);

  comment_code("Method Declaration");
  print_code("\t.align 4\n"
             "\t.globl %s\n"
	     "%s:\n"
             "\tpushl %%ebp\n"
             "\tmovl %%esp, %%ebp\n",
             name, name);

  setCurrentMethod(method_type);
  gencode(mdecl->body);
  setCurrentMethod(NULL);

  print_code("%s$exit:\n"
             "\tpopl %%ebp\n"
             "\tret\n", name);

  free(name);
}

static void
genDestructor(AstNode *root)
{
  Destructor *dest;
  ClassType *class_type;
  char *name, *parentVMT;

  assert(root->type == DESTRUCTOR_NODE);
  dest = (Destructor*)root;

  class_type = getCurClass();
  assert(class_type != NULL);

  name = getDestructorName(class_type->name);

  comment_code("Destructor");
  print_code("\t.align 4\n"
             "\t.globl %s\n"
             "%s:\n"
	     "\tpushl %%ebp\n"
             "\tmovl %%esp, %%ebp\n", 
             name, name);

  constdest = 1;
  gencode(dest->body);
  constdest = 0;

  if(strcmp(class_type->name, OBJECT_NAME) == 0) {
    print_code("%s$exit:\n"
	       "\tpopl %%ebp\n"
	       "\tret\n", name);
  } else {

    parentVMT = getVMTName(&class_type->parent->super); 
    print_code("%s$exit:\n"
	       "\tleal %s, %%eax\n"
	       "\taddl $4, %%eax\n"
	       "\tmovl (%%eax), %%eax\n"
	       "\tcall *%%eax\n"
	       "\tpopl %%ebp\n"
	       "\tret\n", name, parentVMT);

    free(parentVMT);
  }

  free(name);
}

static void
genConstDecl(AstNode *root)
{
  MemberDecl *mdecl;
  MethodType *method_type;
  char *name;

  assert(root->type == CONSTDECL_NODE);
  mdecl = (MemberDecl*)root;
  assert(mdecl->mtype != NULL);
  assert(mdecl->mtype->super.tag == CONSTRUCTOR_TYPE);
  method_type = mdecl->mtype;

  name = getMungedName(method_type);

  comment_code("Constructor Declaration");
  print_code("\t.align 4\n"
             "\t.globl %s\n"
	     "%s:\n"
             "\tpushl %%ebp\n"
             "\tmovl %%esp, %%ebp\n",
             name, name);

  setCurrentMethod(method_type);
  constdest = 1;
  gencode(mdecl->body);
  constdest = 0;
  setCurrentMethod(NULL);

  print_code("%s$exit:\n"
             "\tpopl %%ebp\n"
             "\tret\n", name);

  free(name);
}

static void
genClassCreate(AstNode *root)
{
  ClassCreate *classc;
  ClassType *class_type;
  char *vmt, *constname;

  assert(root->type == CLASSCREATE_NODE);
  classc = (ClassCreate*)root;

  class_type = classc->cons->class;

  vmt = getVMTName(&class_type->super);
  constname = getMungedName(classc->cons);

  gencode(classc->args);
  comment_code("Class Creation");
  print_code("\tpushl $%u\n" /* num args to reverse. */
	     "\tcall RTS_reverseArgumentsOnStack\n"
	     "\taddl $4, %%esp\n"
			 "\tpushl $1\n" /* Number of elements of the given size. */
	     "\tpushl $%u\n" /* size in bytes of the class. */
	     "\tcall calloc\n"
	     "\taddl $8, %%esp\n" /* Remove nelem and size. */
	     "\tpushl %%eax\n"
	     "\tpushl $%u\n" /* line number. */
	     "\tcall RTS_checkMalloc\n"
	     "\taddl $4, %%esp\n"
	     "\tpopl %%eax\n"
	     "\tmovl $%s, (%%eax)\n" /* name of VMT. */
	     "\tpushl %%eax\n" /* Push `this' */
	     "\tcall %s\n" /* constructor name. */
	     "\tpopl %%eax\n" /* Pop `this' */
	     "\taddl $%u, %%esp\n" /* args. */
	     "\tpushl %%eax\n",
	     classc->cons->numParams,
	     (getNumFields(class_type)*4)+4 /* 4 extra for VMT ptr */,
	     classc->super.super.line,
	     vmt,
	     constname,
	     classc->cons->numParams * 4);

  free(constname);
  free(vmt);
}

static void
genParam(AstNode *root)
{
  ExprId *parm;
  MethodType *method_type;
  ParamType *param_type;

  assert(root->type == PARAM_NODE);
  parm = (ExprId*)root;

  method_type = getCurMethod();
  assert(method_type != NULL);

  param_type = getParameter(parm->name, method_type);

  comment_code("Parameter");
  print_code("\tleal %u(%%ebp), %%eax\n"
	     "\tpushl %%eax\n",
	     ((param_type->num-1) * 4) + 12);
}

static void
genDelete(AstNode *root)
{
  DeleteSt *del;

  assert(root->type == DELETE_NODE);
  del = (DeleteSt*)root;

  gencode(del->expr);

  comment_code("Delete");
  print_code("\tpopl %%eax\n"
	     "\tpushl %%eax\n"

	     "\tpushl $%u\n" /* line no */
	     "\tcall RTS_checkNullReference\n"
	     "\taddl $4, %%esp\n"
	     "\tpopl %%eax\n"
	     "\tpushl %%eax\n"
	     "\tmovl (%%eax), %%eax\n"
	     "\taddl $%u, %%eax\n" /* destructor offset. */
	     "\tmovl (%%eax), %%eax\n"
	     "\tcall *%%eax\n"
	     "\tcall free\n"
	     "\taddl $4, %%esp\n",
	     del->super.line,
	     4 /* destructor offset is always 4 */);
}

static void
genFieldAccess(AstNode *root)
{
  FieldAccess *facc;
  Expr *target;
  unsigned int foff;

  assert(root->type == FIELDACCESS_NODE);
  facc = (FieldAccess*)root;

  assert(isExpr(facc->target) == 1);
  target = (Expr*)facc->target;

  assert(target->type->tag == CLASS_TYPE);

  /* Needs an additional 4 to get over the VMT pointer. */
  foff = (getFieldNum((ClassType*)target->type, facc->name) * 4) + 4;

  gencode(facc->target);

  comment_code("Field Access");
  print_code("\tpushl $%u\n" /* line no. */
	     "\tcall RTS_checkNullReference\n"
	     "\taddl $4, %%esp\n"
	     "\tpopl %%eax\n"
	     "\taddl $%u, %%eax\n"
	     "\tpushl %%eax\n",
	     facc->super.super.line, foff);
}

static void
genField(AstNode *root)
{
  ExprId *field;
  ClassType *cur_class;
  unsigned int foff;

  assert(root->type == FIELD_NODE);
  field = (ExprId*)root;

  cur_class = getCurClass();
  assert(cur_class != NULL);

  /* Needs an aditional 4 to get over the VMT. */
  foff = (getFieldNum(cur_class, field->name) * 4)  + 4;

  comment_code("Field");
  print_code("\tmovl 8(%%ebp), %%eax\n"
	     "\taddl $%u, %%eax\n"
	     "\tpushl %%eax\n",
	     foff);
}

static void
genThis(AstNode *root)
{
  Expr *expr;

  assert(root->type == THIS_NODE ||
	 root->type == SUPER_NODE);	
  expr = (Expr*)root;

  if(expr->super.type == THIS_NODE) {
    comment_code("This");
  } else {
    comment_code("Super");
  }

  print_code("\tmovl 8(%%ebp), %%eax\n"
	     "\tpushl %%eax\n");
}

static void
genMethodInvoc(AstNode *root)
{
  MethodInvoc *minvoc;
  Expr *target;
  char *name;
  unsigned int args;
  unsigned int moffs;

  assert(root->type == METHODINVOC_NODE);
  minvoc = (MethodInvoc*)root;

  assert(isExpr(minvoc->target) == 1);
  target = (Expr*)minvoc->target;

  gencode(minvoc->target); /* Target first, */
  gencode(minvoc->args);   /* Then arguments. */

	if(target->type->tag == ARRAY_TYPE) {
		comment_code("Array .equals Invocation");
		print_code("\tcall Object$equals$Object$\n"
               "\taddl $8, %%esp\n"
               "\tpushl %%eax\n");
		return;
	}

  args = minvoc->method_type->numParams;

  if(minvoc->target->type == SUPER_NODE) {
    name = getMungedName(minvoc->method_type);
    comment_code("MethodInvocation");
    print_code("\tpushl $%u\n" /* line number */
	       "\tcall RTS_checkNullReference\n"
	       "\taddl $4, %%esp\n"
	       "\tpushl $%u\n" /* num args to reverse + 1 for `this' */
	       "\tcall RTS_reverseArgumentsOnStack\n"
	       "\taddl $4, %%esp\n"
	       "\tcall %s\n" /* Method name. */
	       "\taddl $%u, %%esp\n" /* size of args to remove + 4 for `this' */
	       "\tpushl %%eax\n", 
	       minvoc->super.super.line, args + 1, name, (args + 1) * 4);
    free(name);

    return;
  }

  /* method number * 4 plus an additional 4 for the super VMT and ANOTHER 4 for
   * the destructor. */
  moffs = (getMethodNum( (ClassType*)target->type, minvoc->method_type) * 4) + 8;

  comment_code("MethodInvocation");
  print_code("\tpushl $%u\n" /* line number */
	     "\tcall RTS_checkNullReference\n"
	     "\taddl $4, %%esp\n"
	     "\tpushl $%u\n" /* num args to reverse + 1 for `this' */
	     "\tcall RTS_reverseArgumentsOnStack\n"
	     "\taddl $4, %%esp\n"
	     "\tpopl %%eax\n"
	     "\tpushl %%eax\n"
	     "\tmovl (%%eax), %%eax\n"
	     "\taddl $%u, %%eax\n" /* method offset. */
	     "\tmovl (%%eax), %%eax\n"
	     "\tcall *%%eax\n"
	     "\taddl $%u, %%esp\n" /* size of args to remove + 4 for `this' */
	     "\tpushl %%eax\n", 
	     minvoc->super.super.line, args + 1, moffs, (args + 1) * 4);
}

static void
genConstInvoc(AstNode *root)
{
  ConstInvoc *invoc;
  char *name;

  assert(root->type == CONSTINVOC_NODE);
  invoc = (ConstInvoc*)root;

  name = getMungedName(invoc->cons);

  gencode(invoc->args);

  comment_code("Constructor Invocation");
  print_code("\tpushl $%u\n" /* num args to reverse. */
	     "\tcall RTS_reverseArgumentsOnStack\n"
	     "\taddl $4, %%esp\n" /* remove arg count. */
	     "\tpushl 8(%%ebp)\n" /* push `this' for some reason */
	     "\tcall %s\n"
	     "\taddl $%u, %%esp\n" /* remove arguments. */,
	     invoc->cons->numParams, name, (invoc->cons->numParams+1) * 4);
  free(name);
}

static void
genCast(AstNode *root)
{
  Cast *cast;
  ClassType *class_type;
  char *dstvmt;

  assert(root->type == CAST_NODE);
  cast = (Cast*)root;

  assert(cast->super.type->tag == CLASS_TYPE);
  class_type = (ClassType*) cast->super.type;

  gencode(cast->expr);

  dstvmt = getVMTName(&class_type->super);
  comment_code("Cast");
  print_code("\tpushl $%s\n" /* dest vmt */
	     "\tpushl $%u\n" /* line number */
	     "\tcall RTS_checkNarrowingConversion\n"
	     "\taddl $8, %%esp\n", 
	     dstvmt, cast->super.super.line);
  free(dstvmt);
}

static void
genArrayCreate(AstNode *root)
{
	ArrayCreate *create;
	char *name;

	assert(root->type == ARRAYCREATE_NODE);
	create = (ArrayCreate*)root;

	gencode(create->expr);

	name = getVMTName(arrayBaseType((ArrayType*)create->super.type));

	comment_code("ArrayCreate");
	print_code("\tpushl $%u\n" /* Number of specified dims */
             "\tpushl $%u\n" /* Rank */
             "\tpushl $%s\n" /* Basetype vmt. */
             "\tpushl $%u\n" /* Line number. */
             "\tcall RTS_createArray\n"
             "\taddl $%u, %%esp\n" /* arg count to RTS_createArray */
             "\tpushl %%eax\n",
             create->rank - create->dims, create->rank, name, 
             create->super.super.line,
             ((create->rank - create->dims) * 4) + 16);
	free(name);
}

static void
genLengthAcc(AstNode * root)
{
	FieldAccess *facc;
	unsigned int foff = 12;

	assert(root->type == LENGTHACC_NODE);
	facc = (FieldAccess*)root;	

	gencode(facc->target);		

	comment_code("Length Access");
	print_code("\tpushl $%u\n" /* line no. */
             "\tcall RTS_checkNullReference\n"
             "\taddl $4, %%esp\n"
             "\tpopl %%eax\n"
             "\taddl $%u, %%eax\n"
             "\tpushl %%eax\n",
             facc->super.super.line, foff);
}

static void
genArrayAcc(AstNode *root)
{
	ArrayAccess *acc;

	assert(root->type == ARRAYACCESS_NODE);
	acc = (ArrayAccess*)root;	

	gencode(acc->name);		
	gencode(acc->dexp);

	comment_code("Length Access");
	print_code("\tpopl %%edx\n" /* dim expr. */
             "\timull $4, %%edx\n"
             "\taddl $16, %%edx\n"
             "\tpushl $%u\n" /* line no. */
             "\tcall RTS_checkNullReference\n"
             "\taddl $4, %%esp\n"
             "\tpopl %%eax\n"
             "\taddl %%edx, %%eax\n"
             "\tpushl %%eax\n",
             acc->super.super.line);
}

static void 
gencode(AstNode *root)
{
  if(root == NULL)
    return;

  switch(root->type) {
  case SEQ_NODE:
    genSeq(root);
    break;

  case MAINFUNC_NODE:
    genMainFunc(root);
    break;

  case MAINVARDECL_NODE:
    genMainVarDecl(root);
    break;

  case MAINVAR_NODE:
    genMainVar(root);
    break;

  case INTLIT_NODE:
    genIntLit(root);
    break;

  case NULLLIT_NODE:
    genNullLit(root);
    break;

  case EXPRST_NODE:
    genExprSt(root);
    break;

  case BINARYOP_NODE:
    genBinaryOp(root);
    break;

  case UNARYOP_NODE:
    genUnaryOp(root);
    break;

  case DEREF_NODE:
    genDeref(root);
    break;

  case OUT_NODE:
    genOut(root);
    break;

  case RETURN_NODE:
    genReturn(root);
    break;

  case IFELSE_NODE:
    genIfElse(root);
    break;

  case WHILE_NODE:
    genWhile(root);
    break;

  case CLASS_NODE:
    genClass(root);
    break;

  case METHDECL_NODE:
    genMethDecl(root);
    break;

  case CONSTDECL_NODE:
    genConstDecl(root);
    break;

  case DESTRUCTOR_NODE:
    genDestructor(root);
    break;

  case CLASSCREATE_NODE:
    genClassCreate(root);
    break;

  case DELETE_NODE:
    genDelete(root);
    break;

  case PARAM_NODE:
    genParam(root);
    break;

  case FIELDACCESS_NODE:
    genFieldAccess(root);
    break;

  case FIELD_NODE:
    genField(root);
    break;

  case SUPER_NODE: /* Fall thru. */
  case THIS_NODE:
    genThis(root);
    break;

  case METHODINVOC_NODE:
    genMethodInvoc(root);
    break;

  case CONSTINVOC_NODE:
    genConstInvoc(root);
    break;

  case CAST_NODE:
    genCast(root);
    break;

	case ARRAYCREATE_NODE:
		genArrayCreate(root);
		break;

	case LENGTHACC_NODE:
		genLengthAcc(root);
		break;

  case ARRAYACCESS_NODE:
		genArrayAcc(root);
		break;

  default:
    break;
  }
}

static void
printVMTs()
{
  ClassType *cp;	
  MethodType *mp;
  List *l, *meths;
  char *vmt, *parentvmt, *dest, *method;

  for(cp = classes; cp != NULL; cp = cp->next) {

    /* VMT start label. */
    vmt = getVMTName(&cp->super);
    parentvmt = getVMTName(&cp->parent->super);
    dest = getDestructorName(cp->name);
    print_code("%s:\n"
	       "\t.long %s\n" 
	       "\t.long %s\n",
	       vmt, parentvmt, dest);
    free(dest);
    free(parentvmt);
    free(vmt);

    /* Pointers to all methods. */
    meths = getMethods(cp);	
    for(l = meths; l != NULL; l = l->next) {
      mp = (MethodType*)l->data;
      assert(mp->super.tag == METHOD_TYPE);
      method = getMungedName(mp);
      print_code("\t.long %s\n",
		 method);
      free(method);
    }
    list_free(meths);

    print_code("\n");
  }	

}

void
output_code(AstNode *root, FILE *outfile)
{
  code_file = outfile;
  comment_code("Run Time System Function Declarations");
  print_code("\t.extern RTS_outputInteger\n");
  print_code("\t.extern RTS_checkForZeroDivide\n");
  print_code("\t.extern RTS_reverseArgumentsOnStack\n");
  print_code("\t.extern RTS_checkMalloc\n");
  print_code("\t.extern RTS_checkNullReference\n");
	print_code("\t.extern RTS_checkNarrowingConversion\n");
	print_code("\t.extern RTS_createArray\n");

  comment_code("VMTs");
  print_code("\t.data\n");
	print_code("\t.globl RTS_arrayVMT\n"
             "RTS_arrayVMT:\n"
             "\t.long 0\n"
             "\t.long Object$$destructor\n"
             "\t.long Object$equals$Object$\n");
  printVMTs();

  print_code("\n");
  print_code("\t.text\n");
  gencode(root);
}
