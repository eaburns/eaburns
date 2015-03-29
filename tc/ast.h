/*
 * ast.h declarations for AST node structures.
 * Ethan Burns Feb 8, 2006 
 */
#ifndef _AST_H_
#define _AST_H_

#include "literal.h"
#include "type.h"

/*
 * AST node types. 
 */
typedef enum {
  SEQ_NODE, INTLIT_NODE, NULLLIT_NODE, ARRAY_NODE, INT_NODE,
  ARRAYACCESS_NODE,
  NAMEID_NODE, METHODINVOC_NODE, SUPER_NODE, FIELDACCESS_NODE, THIS_NODE,
  OUT_NODE, DELETE_NODE, RETURN_NODE, WHILE_NODE, ARRAYCREATE_NODE,
  CLASSCREATE_NODE, EXPRID_NODE, CAST_NODE, UNARYOP_NODE, BINARYOP_NODE,
  IFELSE_NODE, MAINVARDECL_NODE, FORMALPARAM_NODE, DESTRUCTOR_NODE,
  CONSTINVOC_NODE, CONSTDECLATOR_NODE, CONSTDECL_NODE, METHDECLATOR_NODE,
  METHDECL_NODE, FIELDDECL_NODE, CLASS_NODE, MAINFUNC_NODE, DEREF_NODE,
  MAINVAR_NODE, FIELD_NODE, PARAM_NODE, EXPRST_NODE, LENGTHACC_NODE
} NodeType;

/*
 * Binary/unary expression operations. 
 */
typedef enum {
  ASI_OP,                       /* Assignment. */
  EQU_OP,                       /* Equality. */
  LES_OP,                       /* Less than. */
  GRT_OP,                       /* Greater than. */
  ADD_OP,                       /* Addition. */
  SUB_OP,                       /* Subtraction. */
  MUL_OP,                       /* Multiplication. */
  DIV_OP,                       /* Division. */
  NEG_OP,                       /* - Negation. */
  NOT_OP                        /* ! Not. */
} OpType;

/*
 * Basic AST node structure. 
 */
/*
 * INT_NODE: An Integer type node, uses AstNode. 
 */
typedef struct {
  NodeType type;                /* Node type. */
  unsigned int line;            /* Line number. */
} AstNode;

/*
 * The root of the AST. 
 */
extern AstNode *ast;

/*
 * EXPRST_NODE: An expression statement.
 * This seems redundant, however, it is usefull to know if we need to pop an
 * extra return value off of the stack that is left behind by an expression
 * statement.
 */
typedef struct {
  AstNode super;
  AstNode *expr;
} ExprSt;

/*
 * Abstract expression structure. 
 */
/*
 * NULLLIT_NODE: A null literal, uses Expr. 
 */
/*
 * THIS_NODE: A this (keyword) node, uses AstNode. 
 */
/*
 * SUPER_NODE: The super keyword, uses AstNode. 
 */
typedef struct {
  AstNode super;
  Type *type;                   /* Set durring symantic check. */
} Expr;

/*
 * DEREF_NODE: A de-reference node. 
 */
typedef struct {
  Expr super;
  AstNode *next;
} Deref;

/*
 * ARRAYCREATE_NODE: An array creation expression. 
 */
typedef struct {
  Expr super;
  AstNode *type;     /* Type of array. */
  AstNode *expr;     /* Dimension expression. */
  unsigned int dims; /* Unspecified size Dimensions */
	unsigned int rank; /* Total number of specified and unspecified dims. */
} ArrayCreate;

/*
 * CLASSCREATE_NODE: Class instance creation node. 
 */
typedef struct {
  Expr super;
  AstNode *type;                /* Type name of class. */
  AstNode *args;                /* Arguments to constructor. */
  MethodType *cons;             /* The constructor being used. */
} ClassCreate;

/*
 * EXPRID_NODE: An expression identifier. 
 * MAINVAR_NODE: An expression identifier. 
 * FIELD_NODE: An expression identifier. 
 * PARAM_NODE: An expression identifier. 
 */
typedef struct {
  Expr super;
  char *name;                   /* Name of the ident. */
} ExprId;

/*
 * NAMEID_NODE: An expression identifier. 
 */
typedef struct {
  AstNode super;
  char *name;                   /* Name of the ident. */
} NameId;

/*
 * CAST_NODE: A cast expression node. 
 */
typedef struct {
  Expr super;
  AstNode *type;                /* Target type of the cast. */
  AstNode *expr;                /* Expression to cast. */
} Cast;

/*
 * UNARYOP_NODE : Unary operation expression node. NEG_OP, NOT_OP 
 */
typedef struct {
  Expr super;
  OpType op;                    /* Operation type. */
  AstNode *left;                /* Left-hand expression. */
} UnaryOp;

/*
 * BINARYOP_NODE : Binary operation expression node. ASI_OP, EQU_OP,
 * LES_OP, GRT_OP, ADD_OP, SUB_OP, MUL_OP, DIV_OP 
 */
typedef struct {
  Expr super;
  OpType op;                    /* Operation type. */
  AstNode *left;                /* Left-hand expression. */
  AstNode *right;               /* Right-hand expression. */
} BinaryOp;

/*
 * SEQ_NODE: A dummy sequence of nodes. 
 */
typedef struct {
  AstNode super;
  AstNode *data;
  AstNode *next;
} Seq;

/*
 * INTLIT_NODE: An integer ilteral. 
 */
typedef struct {
  Expr super;
  char *str;
} IntLit;

/*
 * ARRAY_NODE: An array. 
 */
typedef struct {
  AstNode super;
  AstNode *child;
} Array;

/*
 * ARRAYACCESS_NODE: An array access. 
 */
typedef struct {
  Expr super;
  AstNode *name;                /* Array name. */
  AstNode *dexp;                /* Dimension expression. */
} ArrayAccess;

/*
 * METHODINVOC_NODE: A method invocation. 
 */
typedef struct {
  Expr super;
  AstNode *target;              /* Object containing the method. */
  char *name;                   /* Method name identifier. */
  AstNode *args;                /* Method arguments. */
  MethodType *method_type;      /* The method type being invoked. */
} MethodInvoc;

/*
 * FIELDACCESS_NODE: A field access node. 
 */
typedef struct {
  Expr super;
  AstNode *target;              /* The object containing the field. */
  char *name;                   /* Field name identifier. */
} FieldAccess;

/*
 * OUT_NODE: An out statement. 
 */
typedef struct {
  AstNode super;
  AstNode *expr;                /* The expression to output. */
} OutSt;

/*
 * DELETE_NODE: A delete statement. 
 */
typedef struct {
  AstNode super;
  AstNode *expr;                /* The expression to delete. */
} DeleteSt;

/*
 * RETURN_NODE: A return statement. 
 */
typedef struct {
  AstNode super;
  AstNode *expr;                /* The expression to return . */
} ReturnSt;

/*
 * WHILE_NODE: A while statement. 
 */
typedef struct {
  AstNode super;
  AstNode *expr;                /* The boolean expression of the while. */
  AstNode *stats;               /* The statements. */
} WhileSt;

/*
 * IFELSE_NODE: An if then else statement. 
 */
typedef struct {
  AstNode super;
  AstNode *expr;                /* The conditional expression. */
  AstNode *tstat;               /* Statements to execute when true. */
  AstNode *fstat;               /* Statements to execute when false. */
} IfElseSt;

/*
 * MAINVARDECL_NODE: A variable declaration node. 
 */
typedef struct {
  AstNode super;
  AstNode *type;                /* Type of the declarators. */
  AstNode *decl;                /* Declarators. */
  AstNode *dims;                /* Dimensions. */
} VarDecl;

/*
 * FORMALPARAM_NODE: A formal parameter. 
 */
typedef struct {
  Expr super;
  AstNode *type;                /* Type of the parameter. */
  AstNode *decl;                /* Declarators. */
  int num;                      /* Number of the parameter in the arg
                                 * list. */
  char *name;                   /* The name of the parameter. */
} FormalParam;

/*
 * DESTRUCTOR_NODE: A destructor. 
 */
typedef struct {
  AstNode super;
  char *name;                   /* Destructor identifier. */
  AstNode *body;                /* Destructor body. */
} Destructor;

/*
 * CONSTINVOC_NODE: A constructor invocation. 
 */
typedef struct {
  Expr super;
  AstNode *target;              /* Target object. */
  AstNode *args;                /* Arguments. */
  MethodType *cons;
} ConstInvoc;

/*
 * METHDECLATOR_NODE: A method declarator node. 
 */
/*
 * CONSTDELCATOR_NODE: A constructor declarator node. 
 */
typedef struct {
  AstNode super;
  char *name;
  AstNode *param;
} MethDeclator;

/*
 * METHDECL_NODE: A method declaration node. 
 */
/*
 * CONSTDECL_NODE: A constructor declaration node. 
 */
/*
 * FIELDDECL_NODE: A field declaration node. 
 */
typedef struct {
  AstNode super;
  AstNode *type;                /* Type of the declaration. */
  AstNode *decl;                /* Declarator. */
  AstNode *body;                /* Body. */
  MethodType *mtype;				    /* Method type pointer. */
  Type *t;                      /* Pointer to the type. */
} MemberDecl;

/*
 * CLASS_NODE: A class declaration node. 
 */
typedef struct {
  AstNode super;
  AstNode *body;                /* Class body. */
  char *pname;                  /* Parent's name. */
  char *name;                   /* Class name. */
} Class;

/*
 * MAINFUNC_NODE: A main function declaration node. 
 */
typedef struct {
  AstNode super;
  AstNode *body;
} MainFunc;

/*
 * Function declarations.
 */
AstNode *makeExprSt(AstNode *expr);
AstNode *makeIntLit(Literal * value);
AstNode *makeNullLit(unsigned int line);
AstNode *makeDeref(Type * type, AstNode * next, unsigned int line);
void freeDeref(Deref * d);      /* Frees a deref's memory. */
AstNode *makeArray(AstNode * child, unsigned int line);
AstNode *makeInt(unsigned int line);
AstNode *makeSeq(AstNode * data);
AstNode *appendSeq(AstNode * child, AstNode * list);
AstNode *makeArrayAccess(AstNode * name, AstNode * dexp,
                         unsigned int line);
AstNode *makeMethodInvoc(AstNode * target, AstNode * ident, AstNode * args,
                         unsigned int line);
AstNode *makeSuper(unsigned int line);
AstNode *makeFieldAccess(AstNode * target, AstNode * ident,
                         unsigned int line);
AstNode *makeThis(unsigned int line);
AstNode *makeOutSt(AstNode * expr, unsigned int line);
AstNode *makeDeleteSt(AstNode * expr, unsigned int line);
AstNode *makeReturnSt(AstNode * expr, unsigned int line);
AstNode *makeWhileSt(AstNode * expr, AstNode * stats, unsigned int line);
AstNode *makeArrayCreate(AstNode * type, AstNode * expr,
                         unsigned int dims, unsigned int line);
AstNode *makeClassCreate(AstNode * type, AstNode * args,
                         unsigned int line);
AstNode *makeExprId(Literal * name);
AstNode *makeCast(AstNode * type, AstNode * expr, unsigned int line);
AstNode *makeUnaryOp(OpType op, AstNode * left, unsigned int line);
AstNode *makeBinaryOp(OpType op, AstNode * left, AstNode * right,
                      unsigned int line);
AstNode *makeIfElseSt(AstNode * expr, AstNode * tstat,
                      AstNode * fstat, unsigned int line);
AstNode *makeMainVarDecl(AstNode * type, AstNode * decl,
                         unsigned int line);
AstNode *makeFormalParam(AstNode * type, AstNode * decl,
                         unsigned int line);
AstNode *makeDestructor(AstNode * ident, AstNode * body,
                        unsigned int line);
AstNode *makeConstInvoc(AstNode * target, AstNode * args,
                        unsigned int line);
AstNode *makeConstDeclator(AstNode * ident, AstNode * param,
                           unsigned int line);
AstNode *makeConstDecl(AstNode * decl, AstNode * body, unsigned int line);
AstNode *makeMethDeclator(AstNode * ident, AstNode * param,
                          unsigned int line);
AstNode *makeMethDecl(AstNode * type, AstNode * decl,
                      AstNode * body, unsigned int line);
AstNode *makeFieldDecl(AstNode * type, AstNode * decl, unsigned int line);
AstNode *makeClass(AstNode * ident, AstNode * parent,
                   AstNode * body, unsigned int line);
AstNode *makeMainFunc(AstNode * body, unsigned int line);
AstNode *makeCUnit(AstNode * list);
AstNode *makeNameId(Literal * name);

/*
 * Returns 1 if any of the current node's children have an error flag set.
 * If not 0 is returned.
 */
int childrenHaveError(AstNode * n);

/*
 * Builds a Type given an AstNode. 
 */
Type *buildType(AstNode * n);

/*
 * Returns 1 if the node is an expression node, else 0 is returned.
 */
int isExpr(AstNode * n);

/*
 * Returns a list of arrays given a declarator. 
 */
Type *getDims(AstNode * n);

/*
 * Returns the identifier given a declarator. 
 */
char *getIdent(AstNode * n);

/*
 * Builds the primordial Object class AST, and returns a pointer to the root.
 */
AstNode *makeObject();

/*
 * Builds a default constructor given the MethodType of the constructor.
 */
AstNode *makeDefaultConstructor(MethodType * cons);

/*
 * Builds a default destructor given the ClassType.
 */
AstNode *makeDefaultDestructor(ClassType * class_type);

/*
 * Prints an AST in prefix notation. 
 */
#define printNode(x) _printNode((x),0)
void _printNode(AstNode * n, unsigned int seqnum);

/*
 * Called before building the AST to setup the memory list. 
 */
void initAst();

/*
 * Frees the memory associated with the AST. 
 */
void freeAst();

#endif                          /* !_AST_H_ */
