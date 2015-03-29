/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     INTEGER_LITERAL = 258,
     IDENTIFIER = 259,
     NULL_LITERAL = 260,
     INT = 261,
     MAIN = 262,
     CLASS = 263,
     EXTENDS = 264,
     THIS = 265,
     SUPER = 266,
     IF = 267,
     ELSE = 268,
     WHILE = 269,
     NEW = 270,
     DELETE = 271,
     RETURN = 272,
     OUT = 273,
     UNKNOWN = 274,
     EQ_OP = 275
   };
#endif
/* Tokens.  */
#define INTEGER_LITERAL 258
#define IDENTIFIER 259
#define NULL_LITERAL 260
#define INT 261
#define MAIN 262
#define CLASS 263
#define EXTENDS 264
#define THIS 265
#define SUPER 266
#define IF 267
#define ELSE 268
#define WHILE 269
#define NEW 270
#define DELETE 271
#define RETURN 272
#define OUT 273
#define UNKNOWN 274
#define EQ_OP 275




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 2068 of yacc.c  */
#line 18 "parse.y"

	unsigned int y_line;
	int 		y_int;
	char		*y_str;
	Literal *y_literal;
	AstNode *y_node;



/* Line 2068 of yacc.c  */
#line 100 "y.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


