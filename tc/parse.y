%{
/*
 * parse.y defines a yacc parser for the T programming language.
 * Ethan Burns -- Created <unknown date>
 * Ethan Burns -- Added better line number support.
 *                Wed Feb 22 17:14:30 EST 2006
 */
#include <stdlib.h>
#include <stdio.h>
#include "ast.h"
#include "literal.h"
#include "mtrack.h"

extern int yylex(); /* This is lame... they should make a header file. */
void yyerror(const char *);
%}

%union {
	unsigned int y_line;
	int 		y_int;
	char		*y_str;
	Literal *y_literal;
	AstNode *y_node;
}

%token <y_literal> INTEGER_LITERAL
%token <y_literal> IDENTIFIER
%token <y_line> NULL_LITERAL
%token <y_line> INT MAIN CLASS EXTENDS THIS SUPER IF ELSE WHILE NEW DELETE 
%token <y_line> RETURN OUT
%token <y_line> UNKNOWN
%token <y_line> '='
%token <y_line> '+'
%token <y_line> EQ_OP
%token <y_line> '>'
%token <y_line> '-'
%token <y_line> '!'
%token <y_line> '/'
%token <y_line> '<'
%token <y_line> '*'
%token <y_line> '('
%token <y_line> ')'
%token <y_line> '{'
%token <y_line> '}'
%token <y_line> '['
%token <y_line> ']'
%token <y_line> ';'
%token <y_line> ','
%token <y_line> '.'
%token <y_line> '~'

/* 
 * Types 
 */
%type <y_literal>  Identifier
%type <y_node> Literal
%type <y_node> IntegralType
%type <y_node> ClassType
%type <y_node> ArrayType
%type <y_node> NumericType
%type <y_node> PrimitiveType
%type <y_node> ReferenceType
%type <y_node> Type
%type <y_node> ArgumentList
%type <y_node> CompilationUnit
%type <y_node> MainFunctionDeclaration
%type <y_node> MainFunctionBody
%type <y_node> ClassDeclarations
%type <y_node> ClassDeclaration
%type <y_node> ClassBody
%type <y_node> ClassBodyDeclarations
%type <y_node> ClassBodyDeclaration
%type <y_node> ClassMemberDeclaration
%type <y_node> FieldDeclaration
%type <y_node> VariableDeclarators
%type <y_node> VariableDeclarator
%type <y_node> MethodDeclaration
%type <y_node> MethodDeclarator
%type <y_node> MethodBody
%type <y_node> ConstructorDeclaration
%type <y_node> ConstructorDeclarator
%type <y_node> ConstructorBody
%type <y_node> ConstructorInvocation
%type <y_node> DestructorDeclaration
%type <y_node> DestructorDeclarator
%type <y_node> DestructorBody
%type <y_node> FormalParameters
%type <y_node> FormalParameterList
%type <y_node> FormalParameter
%type <y_node> VariableDeclaratorID
%type <y_node> Block
%type <y_node> BlockStatements
%type <y_node> BlockStatement
%type <y_node> MainBlock
%type <y_node> MainBlockStatements
%type <y_node> MainBlockStatement
%type <y_node> MainVariableDeclaration
%type <y_node> Statement
%type <y_node> IfThenElseStatement
%type <y_node> WhileStatement
%type <y_node> ReturnStatement
%type <y_node> DeleteStatement
%type <y_node> OutputStatement
%type <y_node> EmptyStatement
%type <y_node> ParenExpression
%type <y_node> ExpressionStatement 
%type <y_node> StatementExpression
%type <y_node> Expression
%type <y_node> AssignmentExpression
%type <y_node> Assignment
%type <y_node> LeftHandSide
%type <y_node> EqualityExpression
%type <y_node> RelationalExpression
%type <y_node> AdditiveExpression
%type <y_node> MultiplicativeExpression
%type <y_node> UnaryExpression
%type <y_node> CastExpression
%type <y_node> Primary
%type <y_node> PrimaryNoNewArray
%type <y_node> ClassInstanceCreationExpression
%type <y_node> ArrayCreationExpression
%type <y_node> DimensionExpressions
%type <y_node> DimensionExpression
%type <y_int>  Dimensions
%type <y_node> Dimension
%type <y_node> FieldAccess
%type <y_node> MethodInvocation
%type <y_node> ArrayAccess
%type <y_node> Arguments
%type <y_node> MainVariableDeclarationStatement

%start CompilationUnit

%%

CompilationUnit	
	: /* empty */
	{}
	| MainFunctionDeclaration
	{
	  $$ = makeCUnit(makeSeq($1));
	}
	| MainFunctionDeclaration ClassDeclarations
	{
	  $$ = makeCUnit(appendSeq($2, makeSeq($1)));
	}
	| ClassDeclarations MainFunctionDeclaration
	{
	  $$ = makeCUnit(appendSeq($1, makeSeq($2)));
	}
	| ClassDeclarations MainFunctionDeclaration ClassDeclarations
	{
	  $$ = makeCUnit(appendSeq($3, appendSeq($1, makeSeq($2))));
	}
	;

MainFunctionDeclaration
	: INT MAIN '(' ')' MainFunctionBody
	{
		$$ = makeMainFunc($5, $2);
	}
	;

MainFunctionBody
	: MainBlock
	{
		$$ = $1;
	}
	;

ClassDeclarations
	: ClassDeclarations ClassDeclaration
	{
		$$ = appendSeq(makeSeq($2), $1);
	}
	| ClassDeclaration
	{
		$$ = makeSeq($1);
	}
	;

ClassDeclaration
	: CLASS Identifier ClassBody
	{
		$$ = makeClass(makeNameId($2), NULL, $3, $1);
		free($2);
	}
	| CLASS Identifier EXTENDS ClassType ClassBody
	{
		$$ = makeClass(makeNameId($2), $4, $5, $1);
		free($2);
	}
	;

ClassBody
	: '{' ClassBodyDeclarations '}'
	{
		$$ = $2;
	}
	| '{' '}'
	{
		$$ = NULL;
	}
	;

ClassBodyDeclarations
	: ClassBodyDeclarations ClassBodyDeclaration
	{
		$$ = appendSeq(makeSeq($2), $1);
	}
	| ClassBodyDeclaration
	{
		$$ = makeSeq($1);
	}
	;

ClassBodyDeclaration
	: ClassMemberDeclaration
	{
		$$ = $1;
	}
	| ConstructorDeclaration
	{
		$$ = $1;
	}
	| DestructorDeclaration
	{
		$$ = $1;
	}
	| ';'
	{
		$$ = NULL;
	}
	;

ClassMemberDeclaration
	: FieldDeclaration
	{
		$$ = $1;
	}
	| MethodDeclaration
	{
		$$ = $1;
	}
	;

FieldDeclaration	
	: Type VariableDeclarators ';'
	{
		$$ = makeFieldDecl($1, $2, $3);
	}
	;

VariableDeclarators
	: VariableDeclarators ',' VariableDeclarator
	{
		$$ = appendSeq(makeSeq($3), $1);	
	}
	| VariableDeclarator
	{
		$$ = makeSeq($1);	
	}
	;

VariableDeclarator 
	: VariableDeclarator Dimension
	{
		$$ = makeArray($1, $1->line);
	}
	| Identifier
	{
		$$ = makeNameId($1);
		free($1);
	}
	;

MethodDeclaration	
	: Type MethodDeclarator MethodBody
	{
		$$ = makeMethDecl($1, $2, $3, $1->line);
	}
	;

MethodDeclarator
	: Identifier FormalParameters
	{
		$$ = makeMethDeclator(makeNameId($1), $2, $1->line);
		free($1);
	}
	| MethodDeclarator Dimension 
	{
		$$ = makeArray($1, $1->line);
	}
	;

MethodBody
	: Block
	{
		$$ = $1;
	}
	;

ConstructorDeclaration
	: ConstructorDeclarator ConstructorBody
	{
		$$ = makeConstDecl($1, $2, $1->line);
	}
	;

ConstructorDeclarator
	: Identifier FormalParameters
	{
		$$ = makeConstDeclator(makeNameId($1), $2, $1->line);
		free($1);
	}
	;

ConstructorBody
	: '{' ConstructorInvocation BlockStatements '}'
	{
		$$ = appendSeq($3, makeSeq($2));
	}
	| '{' ConstructorInvocation '}'
	{
		$$ = makeSeq($2);
	}
	| Block
	{
	  $$ = $1;
	}
	;

ConstructorInvocation
	: THIS Arguments ';'
	{
		$$ = makeConstInvoc(makeThis($1), $2, $1);
	}
	| SUPER Arguments ';'
	{
		$$ = makeConstInvoc(makeSuper($1), $2, $1);
	}
	;

DestructorDeclaration
	: DestructorDeclarator DestructorBody
	{
		$$ = makeDestructor($1, $2, $1->line);
	}
	;

DestructorDeclarator
	: '~' Identifier '(' ')'
	{
		$$ = makeNameId($2);
		free($2);
	}
	;

DestructorBody
	: Block
	{
		$$ = $1;
	}
	;

FormalParameters
	: '(' FormalParameterList ')'
	{
		$$ = $2;
	}
	| '(' ')'
	{
		$$ = NULL;
	}
	;

FormalParameterList
	: FormalParameterList ',' FormalParameter
	{
		$$ = appendSeq(makeSeq($3), $1);
	}
	| FormalParameter
	{
		$$ = makeSeq($1);
	}
	;

FormalParameter 
	: Type VariableDeclaratorID
	{
		$$ = makeFormalParam($1, $2, $1->line);
	}
	;

VariableDeclaratorID
	: VariableDeclaratorID Dimension
	{
		$$ = makeArray($1, $1->line);
	}
	| Identifier
	{
		$$ = makeNameId($1);
		free($1);
	}
	;

Block
	: '{' BlockStatements '}'
	{
		$$ = $2;
	}
	| '{' '}'
	{
		$$ = NULL;
	}
	;

BlockStatements	
	: BlockStatements BlockStatement
	{
		$$ = appendSeq(makeSeq($2), $1);
	}
	| BlockStatement
	{
		$$ = makeSeq($1);
	}
	;

BlockStatement
	: Statement
	{
		$$ = $1;
	}
	;

MainBlock
	: '{' MainBlockStatements '}'
	{
		$$ = $2;
	}
	| '{' '}'
	{
		$$ = NULL;
	}
	;

MainBlockStatements	
	: MainBlockStatements MainBlockStatement
	{
		$$ = appendSeq(makeSeq($2), $1);
	}
	| MainBlockStatement
	{
		$$ = makeSeq($1);
	}
	;

MainBlockStatement
	: MainVariableDeclarationStatement
	{
		$$ = $1;
	}
	| BlockStatement
	{
		$$ = $1;
	}
	;

MainVariableDeclarationStatement
	: MainVariableDeclaration ';'
	{
		$$ = $1;
	}
	;

MainVariableDeclaration
	: Type VariableDeclarators
	{
		$$ = makeMainVarDecl($1, $2, $1->line);
	}
	;

Statement	
	: Block
	{
		$$ = $1;
	}
	| EmptyStatement
	{
		$$ = $1;
	}
	| ExpressionStatement
	{
		$$ = $1;
	}
	| IfThenElseStatement
	{
		$$ = $1;
	}
	| WhileStatement
	{
		$$ = $1;
	}
	| ReturnStatement
	{
		$$ = $1;
	}
	| DeleteStatement
	{
		$$ = $1;
	}
	| OutputStatement
	{
		$$ = $1;
	}
	;

IfThenElseStatement
	: IF ParenExpression Statement ELSE Statement
	{
		$$ = makeIfElseSt($2, $3, $5, $1);
	}
	;

WhileStatement
	: WHILE ParenExpression Statement
	{
		$$ = makeWhileSt($2, $3, $1);
	}
	;

ReturnStatement
	: RETURN ';'
	{
		$$ = makeReturnSt(NULL, $1);
	}
	| RETURN Expression ';'
	{
		$$ = makeReturnSt($2, $1);
	}
	;

DeleteStatement
	: DELETE Expression ';'
	{
		$$ = makeDeleteSt($2, $1);
	}
	;

OutputStatement
	: OUT Expression ';'
	{
		$$ = makeOutSt($2, $1);
	}
	;

EmptyStatement
	: ';'
	{
		$$ = NULL;
	}
	;

ParenExpression
	: '(' Expression ')'
	{
		$$ = $2;
	}
	;

ExpressionStatement 
	: StatementExpression ';'
	{
		$$ = makeExprSt($1);
	}
	;

StatementExpression
	: Assignment
	{
		$$ = $1;
	}
	| MethodInvocation
	{
		$$ = $1;
	}
	;

Expression
	: AssignmentExpression
	{
		$$ = $1;
	}
	;

AssignmentExpression
	: Assignment
	{
		$$ = $1;
	}
	| EqualityExpression
	{
		$$ = $1;
	}
	;

Assignment
	: LeftHandSide AssignmentOperator AssignmentExpression
	{
		$$ = makeBinaryOp(ASI_OP, $1, $3, $1->line);
	}
	;

LeftHandSide
	: Identifier
	{
		$$ = makeExprId($1);
		free($1);
	}
	| FieldAccess
	{
		$$ = $1;
	}
	| ArrayAccess
	{
		$$ = $1;
	}
	;

EqualityExpression
	: EqualityExpression EQ_OP RelationalExpression
	{
		$$ = makeBinaryOp(EQU_OP, $1, $3, $2);
	}
	| RelationalExpression
	{
		$$ = $1;
	}
	;

RelationalExpression
	: RelationalExpression '<' AdditiveExpression
	{
		$$ = makeBinaryOp(LES_OP, $1, $3, $2);
	}
	| RelationalExpression '>' AdditiveExpression
	{
		$$ = makeBinaryOp(GRT_OP, $1, $3, $2);
	}
	| AdditiveExpression
	{
		$$ = $1;
	}
	;

AdditiveExpression
	: AdditiveExpression '+' MultiplicativeExpression
	{
		$$ = makeBinaryOp(ADD_OP, $1, $3, $2);
	}
	| AdditiveExpression '-' MultiplicativeExpression
	{
		$$ = makeBinaryOp(SUB_OP, $1, $3, $2);
	}
	| MultiplicativeExpression
	{
		$$ = $1;
	}
	;

MultiplicativeExpression
	: MultiplicativeExpression '*' UnaryExpression
	{
		$$ = makeBinaryOp(MUL_OP, $1, $3, $2);
	}
	| MultiplicativeExpression '/' UnaryExpression
	{
		$$ = makeBinaryOp(DIV_OP, $1, $3, $2);
	}
	| UnaryExpression
	{
		$$ = $1;
	}
	;

UnaryExpression	
	: '-' UnaryExpression
	{
		$$ = makeUnaryOp(NEG_OP, $2, $1);
	}
	| '!' UnaryExpression
	{
		$$ = makeUnaryOp(NOT_OP, $2, $1);
	}
	| CastExpression
	{
		$$ = $1;
	}
	;

CastExpression
	: ParenExpression CastExpression
	{
		$$ = makeCast($1, $2, $1->line);
	}
	| '(' ArrayType ')' CastExpression
	{
		$$ = makeCast($2, $4, $1);
	}
	| Primary
	{
		$$ = $1;
	}
	;

Primary
	: ArrayCreationExpression
	{
		$$ = $1;
	}
	| Identifier
	{
		$$ = makeExprId($1);
		free($1);
	}
	| PrimaryNoNewArray
	{
		$$ = $1;
	}
	;

PrimaryNoNewArray
	: ParenExpression
	{
		$$ = $1;
	}
	| THIS
	{
		$$ = makeThis($1);
	}
	| FieldAccess
	{
		$$ = $1;
	}
	| MethodInvocation
	{
		$$ = $1;
	}
	| ArrayAccess
	{
		$$ = $1;
	}
	| ClassInstanceCreationExpression
	{
		$$ = $1;
	}
	| Literal
	{
		$$ = $1;
	}
	;

ClassInstanceCreationExpression
	: NEW ClassType Arguments
	{
		$$ = makeClassCreate($2, $3, $1);
	}
	;
	
ArrayCreationExpression
	: NEW ClassType DimensionExpressions Dimensions
	{
		$$ = makeArrayCreate($2, $3, $4, $1);
	}
	| NEW ClassType DimensionExpressions
	{
		$$ = makeArrayCreate($2, $3, 0, $1);
	}
	| NEW PrimitiveType DimensionExpressions Dimensions
	{
		$$ = makeArrayCreate($2, $3, $4, $1);
	}
	| NEW PrimitiveType DimensionExpressions
	{
		$$ = makeArrayCreate($2, $3, 0, $1);
	}
	;

DimensionExpressions
	: DimensionExpressions DimensionExpression
	{
		$$ = appendSeq(makeSeq($2), $1);
	}
	| DimensionExpression
	{
		$$ = makeSeq($1);
	}
	;

DimensionExpression
	: '[' Expression ']'
	{
		$$ = $2;
	}
	;

Dimensions
	: Dimensions Dimension
	{
		$$ = $1 + 1;
	}
	| Dimension
	{
		$$ = 1;
	}
	;

Dimension
	: '[' ']'
	{}
	;

FieldAccess
	: Primary '.' Identifier
	{
		$$ = makeFieldAccess($1, makeNameId($3), $2);
		free($3);
	}
	| SUPER '.' Identifier
	{
		$$ = makeFieldAccess(makeSuper($1), makeNameId($3), $2);
		free($3);
	}
	;

MethodInvocation
	: Identifier Arguments
	{
		$$ = makeMethodInvoc(NULL, makeNameId($1), $2, $1->line);
		free($1);
	}
	| Primary '.' Identifier Arguments
	{
		$$ = makeMethodInvoc($1, makeNameId($3), $4, $2);
		free($3);
	}
	| SUPER '.' Identifier Arguments
	{
		$$ = makeMethodInvoc(makeSuper($1), makeNameId($3), $4, $1);
		free($3);
	}
	;

ArrayAccess
	: Identifier DimensionExpression
	{
		$$ = makeArrayAccess(makeExprId($1), $2, $1->line);
		free($1);
	}
	| PrimaryNoNewArray DimensionExpression
	{
		$$ = makeArrayAccess($1, $2, $1->line);
	}
	;

Arguments
	: '(' ArgumentList ')'
	{
		$$ = $2;
	}
	| '(' ')'
	{
		$$ = NULL;
	}
	;

ArgumentList
	: ArgumentList ',' Expression
	{
		$$ = appendSeq(makeSeq($3), $1);
	}
	| Expression
	{
		$$ = makeSeq($1);
	}
	;

AssignmentOperator
	: '='
	{}
	;

Type
	: ReferenceType
	{
		$$ = $1;
	}
	| PrimitiveType
	{
		$$ = $1;
	}
	;

PrimitiveType
	: NumericType
	{
		$$ = $1;
	}
	;

NumericType
	: IntegralType
	{
		$$ = $1;
	}
	;

IntegralType
	: INT
	{
		$$ = makeInt($1);
	}
	;

ReferenceType	
	: ClassType
	{
		$$ = $1;
	}
	| ArrayType
	{
		$$ = $1;
	}
	;

ClassType
	: Identifier
	{
		$$ = makeNameId($1);
		free($1);
	}
	;

ArrayType
	: PrimitiveType Dimension
	{
		$$ = makeArray($1, $1->line);
	}
	| Identifier Dimension
	{
		$$ = makeArray(makeNameId($1), $1->line);
		free($1);
	}
	| ArrayType Dimension
	{
		$$ = makeArray($1, $1->line);
	}
	;

Identifier
	: IDENTIFIER
	{
		$$ = $1;
	}
	;

Literal
	: INTEGER_LITERAL
	{
		$$ = makeIntLit($1);
	}
	| NULL_LITERAL
	{
		$$ = makeNullLit($1);
	}
	;

%%

void yyerror(const char *s)
{
  extern unsigned int line; /* scan.l */
  
  fprintf(stderr, "Line %d: %s\n", line, s);
  t_free_alld();

  exit(1);
}
