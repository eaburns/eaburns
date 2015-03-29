/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison implementation for Yacc-like parsers in C
   
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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 268 of yacc.c  */
#line 1 "parse.y"

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


/* Line 268 of yacc.c  */
#line 89 "y.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


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

/* Line 293 of yacc.c  */
#line 18 "parse.y"

	unsigned int y_line;
	int 		y_int;
	char		*y_str;
	Literal *y_literal;
	AstNode *y_node;



/* Line 293 of yacc.c  */
#line 175 "y.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 187 "y.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  10
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   588

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  39
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  78
/* YYNRULES -- Number of rules.  */
#define YYNRULES  151
/* YYNRULES -- Number of states.  */
#define YYNSTATES  239

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   275

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    25,     2,     2,     2,     2,     2,     2,
      29,    30,    28,    21,    36,    24,    37,    26,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    35,
      27,    20,    23,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    33,     2,    34,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    31,     2,    32,    38,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    22
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     6,     9,    12,    16,    22,    24,
      27,    29,    33,    39,    43,    46,    49,    51,    53,    55,
      57,    59,    61,    63,    67,    71,    73,    76,    78,    82,
      85,    88,    90,    93,    96,   101,   105,   107,   111,   115,
     118,   123,   125,   129,   132,   136,   138,   141,   144,   146,
     150,   153,   156,   158,   160,   164,   167,   170,   172,   174,
     176,   179,   182,   184,   186,   188,   190,   192,   194,   196,
     198,   204,   208,   211,   215,   219,   223,   225,   229,   232,
     234,   236,   238,   240,   242,   246,   248,   250,   252,   256,
     258,   262,   266,   268,   272,   276,   278,   282,   286,   288,
     291,   294,   296,   299,   304,   306,   308,   310,   312,   314,
     316,   318,   320,   322,   324,   326,   330,   335,   339,   344,
     348,   351,   353,   357,   360,   362,   365,   369,   373,   376,
     381,   386,   389,   392,   396,   399,   403,   405,   407,   409,
     411,   413,   415,   417,   419,   421,   423,   426,   429,   432,
     434,   436
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      40,     0,    -1,    -1,    41,    -1,    41,    43,    -1,    43,
      41,    -1,    43,    41,    43,    -1,     6,     7,    29,    30,
      42,    -1,    69,    -1,    43,    44,    -1,    44,    -1,     8,
     115,    45,    -1,     8,   115,     9,   113,    45,    -1,    31,
      46,    32,    -1,    31,    32,    -1,    46,    47,    -1,    47,
      -1,    48,    -1,    55,    -1,    59,    -1,    35,    -1,    49,
      -1,    52,    -1,   108,    50,    35,    -1,    50,    36,    51,
      -1,    51,    -1,    51,   101,    -1,   115,    -1,   108,    53,
      54,    -1,   115,    62,    -1,    53,   101,    -1,    66,    -1,
      56,    57,    -1,   115,    62,    -1,    31,    58,    67,    32,
      -1,    31,    58,    32,    -1,    66,    -1,    10,   105,    35,
      -1,    11,   105,    35,    -1,    60,    61,    -1,    38,   115,
      29,    30,    -1,    66,    -1,    29,    63,    30,    -1,    29,
      30,    -1,    63,    36,    64,    -1,    64,    -1,   108,    65,
      -1,    65,   101,    -1,   115,    -1,    31,    67,    32,    -1,
      31,    32,    -1,    67,    68,    -1,    68,    -1,    74,    -1,
      31,    70,    32,    -1,    31,    32,    -1,    70,    71,    -1,
      71,    -1,    72,    -1,    68,    -1,    73,    35,    -1,   108,
      50,    -1,    66,    -1,    80,    -1,    82,    -1,    75,    -1,
      76,    -1,    77,    -1,    78,    -1,    79,    -1,    12,    81,
      74,    13,    74,    -1,    14,    81,    74,    -1,    17,    35,
      -1,    17,    84,    35,    -1,    16,    84,    35,    -1,    18,
      84,    35,    -1,    35,    -1,    29,    84,    30,    -1,    83,
      35,    -1,    86,    -1,   103,    -1,    85,    -1,    86,    -1,
      88,    -1,    87,   107,    85,    -1,   115,    -1,   102,    -1,
     104,    -1,    88,    22,    89,    -1,    89,    -1,    89,    27,
      90,    -1,    89,    23,    90,    -1,    90,    -1,    90,    21,
      91,    -1,    90,    24,    91,    -1,    91,    -1,    91,    28,
      92,    -1,    91,    26,    92,    -1,    92,    -1,    24,    92,
      -1,    25,    92,    -1,    93,    -1,    81,    93,    -1,    29,
     114,    30,    93,    -1,    94,    -1,    97,    -1,   115,    -1,
      95,    -1,    81,    -1,    10,    -1,   102,    -1,   103,    -1,
     104,    -1,    96,    -1,   116,    -1,    15,   113,   105,    -1,
      15,   113,    98,   100,    -1,    15,   113,    98,    -1,    15,
     109,    98,   100,    -1,    15,   109,    98,    -1,    98,    99,
      -1,    99,    -1,    33,    84,    34,    -1,   100,   101,    -1,
     101,    -1,    33,    34,    -1,    94,    37,   115,    -1,    11,
      37,   115,    -1,   115,   105,    -1,    94,    37,   115,   105,
      -1,    11,    37,   115,   105,    -1,   115,    99,    -1,    95,
      99,    -1,    29,   106,    30,    -1,    29,    30,    -1,   106,
      36,    84,    -1,    84,    -1,    20,    -1,   112,    -1,   109,
      -1,   110,    -1,   111,    -1,     6,    -1,   113,    -1,   114,
      -1,   115,    -1,   109,   101,    -1,   115,   101,    -1,   114,
     101,    -1,     4,    -1,     3,    -1,     5,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   138,   138,   139,   143,   147,   151,   158,   165,   172,
     176,   183,   188,   196,   200,   207,   211,   218,   222,   226,
     230,   237,   241,   248,   255,   259,   266,   270,   278,   285,
     290,   297,   304,   311,   319,   323,   327,   334,   338,   345,
     352,   360,   367,   371,   378,   382,   389,   396,   400,   408,
     412,   419,   423,   430,   437,   441,   448,   452,   459,   463,
     470,   477,   484,   488,   492,   496,   500,   504,   508,   512,
     519,   526,   533,   537,   544,   551,   558,   565,   572,   579,
     583,   590,   597,   601,   608,   615,   620,   624,   631,   635,
     642,   646,   650,   657,   661,   665,   672,   676,   680,   687,
     691,   695,   702,   706,   710,   717,   721,   726,   733,   737,
     741,   745,   749,   753,   757,   764,   771,   775,   779,   783,
     790,   794,   801,   808,   812,   819,   824,   829,   837,   842,
     847,   855,   860,   867,   871,   878,   882,   889,   894,   898,
     905,   912,   919,   926,   930,   937,   945,   949,   954,   961,
     968,   972
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INTEGER_LITERAL", "IDENTIFIER",
  "NULL_LITERAL", "INT", "MAIN", "CLASS", "EXTENDS", "THIS", "SUPER", "IF",
  "ELSE", "WHILE", "NEW", "DELETE", "RETURN", "OUT", "UNKNOWN", "'='",
  "'+'", "EQ_OP", "'>'", "'-'", "'!'", "'/'", "'<'", "'*'", "'('", "')'",
  "'{'", "'}'", "'['", "']'", "';'", "','", "'.'", "'~'", "$accept",
  "CompilationUnit", "MainFunctionDeclaration", "MainFunctionBody",
  "ClassDeclarations", "ClassDeclaration", "ClassBody",
  "ClassBodyDeclarations", "ClassBodyDeclaration",
  "ClassMemberDeclaration", "FieldDeclaration", "VariableDeclarators",
  "VariableDeclarator", "MethodDeclaration", "MethodDeclarator",
  "MethodBody", "ConstructorDeclaration", "ConstructorDeclarator",
  "ConstructorBody", "ConstructorInvocation", "DestructorDeclaration",
  "DestructorDeclarator", "DestructorBody", "FormalParameters",
  "FormalParameterList", "FormalParameter", "VariableDeclaratorID",
  "Block", "BlockStatements", "BlockStatement", "MainBlock",
  "MainBlockStatements", "MainBlockStatement",
  "MainVariableDeclarationStatement", "MainVariableDeclaration",
  "Statement", "IfThenElseStatement", "WhileStatement", "ReturnStatement",
  "DeleteStatement", "OutputStatement", "EmptyStatement",
  "ParenExpression", "ExpressionStatement", "StatementExpression",
  "Expression", "AssignmentExpression", "Assignment", "LeftHandSide",
  "EqualityExpression", "RelationalExpression", "AdditiveExpression",
  "MultiplicativeExpression", "UnaryExpression", "CastExpression",
  "Primary", "PrimaryNoNewArray", "ClassInstanceCreationExpression",
  "ArrayCreationExpression", "DimensionExpressions", "DimensionExpression",
  "Dimensions", "Dimension", "FieldAccess", "MethodInvocation",
  "ArrayAccess", "Arguments", "ArgumentList", "AssignmentOperator", "Type",
  "PrimitiveType", "NumericType", "IntegralType", "ReferenceType",
  "ClassType", "ArrayType", "Identifier", "Literal", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
      61,    43,   275,    62,    45,    33,    47,    60,    42,    40,
      41,   123,   125,    91,    93,    59,    44,    46,   126
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    39,    40,    40,    40,    40,    40,    41,    42,    43,
      43,    44,    44,    45,    45,    46,    46,    47,    47,    47,
      47,    48,    48,    49,    50,    50,    51,    51,    52,    53,
      53,    54,    55,    56,    57,    57,    57,    58,    58,    59,
      60,    61,    62,    62,    63,    63,    64,    65,    65,    66,
      66,    67,    67,    68,    69,    69,    70,    70,    71,    71,
      72,    73,    74,    74,    74,    74,    74,    74,    74,    74,
      75,    76,    77,    77,    78,    79,    80,    81,    82,    83,
      83,    84,    85,    85,    86,    87,    87,    87,    88,    88,
      89,    89,    89,    90,    90,    90,    91,    91,    91,    92,
      92,    92,    93,    93,    93,    94,    94,    94,    95,    95,
      95,    95,    95,    95,    95,    96,    97,    97,    97,    97,
      98,    98,    99,   100,   100,   101,   102,   102,   103,   103,
     103,   104,   104,   105,   105,   106,   106,   107,   108,   108,
     109,   110,   111,   112,   112,   113,   114,   114,   114,   115,
     116,   116
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     1,     2,     2,     3,     5,     1,     2,
       1,     3,     5,     3,     2,     2,     1,     1,     1,     1,
       1,     1,     1,     3,     3,     1,     2,     1,     3,     2,
       2,     1,     2,     2,     4,     3,     1,     3,     3,     2,
       4,     1,     3,     2,     3,     1,     2,     2,     1,     3,
       2,     2,     1,     1,     3,     2,     2,     1,     1,     1,
       2,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       5,     3,     2,     3,     3,     3,     1,     3,     2,     1,
       1,     1,     1,     1,     3,     1,     1,     1,     3,     1,
       3,     3,     1,     3,     3,     1,     3,     3,     1,     2,
       2,     1,     2,     4,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     4,     3,     4,     3,
       2,     1,     3,     2,     1,     2,     3,     3,     2,     4,
       4,     2,     2,     3,     2,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     2,     2,     1,
       1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     0,     0,     3,     0,    10,     0,   149,     0,
       1,     4,     5,     9,     0,     0,     0,    11,     6,     0,
       0,   145,   142,    14,    20,     0,     0,    16,    17,    21,
      22,    18,     0,    19,     0,     0,   139,   140,   141,   138,
     143,   144,   145,     0,     7,     8,    12,     0,    13,    15,
       0,    32,    36,     0,    39,    41,     0,    25,     0,    27,
       0,   146,   148,     0,    33,   147,   150,   151,   109,     0,
       0,     0,     0,     0,     0,     0,     0,    55,    76,    62,
      59,     0,    57,    58,     0,    53,    65,    66,    67,    68,
      69,    63,   108,    64,     0,    79,     0,     0,   107,   113,
     105,   110,   111,   112,     0,    85,   114,     0,   109,     0,
      50,     0,     0,    52,   106,    23,     0,    26,    28,    31,
      30,    29,   125,    43,     0,    45,     0,   145,     0,     0,
       0,     0,     0,     0,     0,     0,   108,     0,    81,    82,
      83,    89,    92,    95,    98,   101,   104,   111,    72,     0,
       0,     0,    54,    56,    60,    78,   137,     0,     0,     0,
     132,    61,    27,     0,     0,   131,   128,    40,     0,     0,
      35,     0,    49,    51,    24,    42,     0,    46,    48,   127,
       0,    71,   119,   121,   117,   115,    99,   110,   112,   106,
     100,     0,     0,   106,   102,    74,     0,     0,     0,     0,
       0,     0,     0,    73,    75,    77,    84,   126,     0,   134,
     136,     0,    37,    38,    34,    44,    47,   130,     0,   120,
     118,   124,   116,     0,    88,    91,    90,    93,    94,    97,
      96,   129,   122,   133,     0,    70,   123,   103,   135
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,    44,     5,     6,    17,    26,    27,    28,
      29,    56,    57,    30,    58,   118,    31,    32,    51,   111,
      33,    34,    54,    64,   124,   125,   177,    79,   112,   113,
      45,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,   136,    93,    94,   151,   138,    95,    96,   140,
     141,   142,   143,   144,   145,   146,    98,    99,   100,   182,
     165,   220,    65,   101,   147,   103,   166,   211,   157,    35,
      36,    37,    38,    39,    40,    41,   114,   106
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -131
static const yytype_int16 yypact[] =
{
     129,    66,    35,    82,    93,   129,  -131,    79,  -131,    18,
    -131,    93,    93,  -131,   111,    35,    80,  -131,    93,   120,
     126,  -131,  -131,  -131,  -131,    35,   123,  -131,  -131,  -131,
    -131,  -131,   147,  -131,   150,    35,   152,  -131,  -131,  -131,
    -131,   152,   -22,   246,  -131,  -131,  -131,   157,  -131,  -131,
     312,  -131,  -131,   345,  -131,  -131,    53,   152,     7,   160,
     158,  -131,  -131,    28,  -131,  -131,  -131,  -131,  -131,   154,
     164,   164,   142,   559,   494,   559,   559,  -131,  -131,  -131,
    -131,   279,  -131,  -131,   166,  -131,  -131,  -131,  -131,  -131,
    -131,  -131,  -131,  -131,   167,  -131,   184,   168,   173,  -131,
    -131,   187,   174,   188,    35,    38,  -131,   183,   185,    74,
    -131,   378,   411,  -131,    24,  -131,    35,  -131,  -131,  -131,
    -131,  -131,  -131,  -131,     1,  -131,    35,   152,    35,   477,
     477,   173,    22,   559,   559,   543,   207,   180,  -131,  -131,
     194,    96,    86,   156,  -131,  -131,   168,  -131,  -131,   190,
     191,   189,  -131,  -131,  -131,  -131,  -131,   559,    35,   559,
    -131,   199,  -131,   527,   510,  -131,  -131,  -131,   202,   203,
    -131,   444,  -131,  -131,   152,  -131,   142,   152,  -131,   185,
     210,  -131,   213,  -131,   213,  -131,  -131,  -131,  -131,    22,
    -131,   152,   140,    27,  -131,  -131,   559,   559,   559,   559,
     559,   559,   559,  -131,  -131,  -131,  -131,   185,   186,  -131,
    -131,    42,  -131,  -131,  -131,  -131,  -131,  -131,   477,  -131,
     152,  -131,   152,   207,    96,    86,    86,   156,   156,  -131,
    -131,  -131,  -131,  -131,   559,  -131,  -131,  -131,  -131
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -131,  -131,   242,  -131,   118,    95,   228,  -131,   227,  -131,
    -131,   155,   138,  -131,  -131,  -131,  -131,  -131,  -131,  -131,
    -131,  -131,  -131,   206,  -131,    91,  -131,    11,   159,   -27,
    -131,  -131,   192,  -131,  -131,  -109,  -131,  -131,  -131,  -131,
    -131,  -131,   -35,  -131,  -131,   -72,   112,   -10,  -131,  -131,
      72,   -34,   -28,  -105,  -130,   -31,  -131,  -131,  -131,   139,
     -15,    88,   -32,    32,     9,    43,    -4,  -131,  -131,   -33,
     -67,  -131,  -131,  -131,     2,   141,    -2,  -131
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -146
static const yytype_int16 yytable[] =
{
       9,   137,   149,   150,    61,   131,   194,    63,    92,    62,
     104,    60,    97,    21,    42,    92,    80,    20,    92,    97,
     180,   181,    97,    47,    42,   117,   120,    15,   186,   190,
     126,   175,     8,    59,    22,   129,   130,   176,    53,     8,
      60,   105,  -145,    52,   -85,    55,    92,   -85,   104,    16,
      97,   163,   102,   163,    80,   159,   163,   159,   123,   102,
     164,   127,   102,   139,   139,   139,   139,   163,   191,   119,
      21,   164,   233,     7,   132,  -106,    92,    92,   234,   105,
      97,    97,    10,   160,     8,   173,    22,   208,   115,   116,
     102,   210,   208,   237,    92,    92,   229,   230,    97,    97,
      13,     2,   162,   163,   168,   169,    13,   199,    14,   235,
     200,   128,    23,    13,   162,    24,   183,   183,    25,   197,
     102,   102,    11,   198,   178,   139,   179,     8,   185,    22,
      18,   189,   189,   193,   189,     1,    92,     2,   102,   102,
      97,    19,   117,   126,   173,   216,     8,   139,    22,   139,
     221,    43,   221,   139,   139,    48,   207,    16,    24,    61,
      62,    25,   238,   225,   226,   187,   187,   219,   187,   219,
     223,   227,   228,    60,   127,   217,   188,   188,    50,   188,
     102,    53,   201,    92,   202,    60,   107,    97,   236,    63,
     236,   128,   122,    76,   189,   189,   189,   189,   189,   189,
     189,   154,   155,   231,   156,   158,   159,   -86,   -87,   -80,
      66,     8,    67,   167,   163,   195,   196,    68,    69,   205,
     232,   189,    72,   218,   139,   203,   204,   102,   187,   187,
     187,   187,   187,   187,   187,   116,   135,   212,   213,   188,
     188,   188,   188,   188,   188,   188,   164,    12,    46,    66,
       8,    67,    22,    49,   174,   187,    68,    69,    70,   161,
      71,    72,    73,    74,    75,   121,   188,   215,   224,   206,
     171,   184,   222,   153,     0,    76,   192,    53,    77,     0,
       0,    78,    66,     8,    67,    22,     0,     0,     0,    68,
      69,    70,     0,    71,    72,    73,    74,    75,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    76,     0,
      53,   152,     0,     0,    78,    66,     8,    67,     0,     0,
       0,     0,   108,   109,    70,     0,    71,    72,    73,    74,
      75,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    76,     0,    53,   110,     0,     0,    78,    66,     8,
      67,     0,     0,     0,     0,    68,    69,    70,     0,    71,
      72,    73,    74,    75,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    76,     0,    53,   110,     0,     0,
      78,    66,     8,    67,     0,     0,     0,     0,    68,    69,
      70,     0,    71,    72,    73,    74,    75,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    76,     0,    53,
     170,     0,     0,    78,    66,     8,    67,     0,     0,     0,
       0,    68,    69,    70,     0,    71,    72,    73,    74,    75,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      76,     0,    53,   172,     0,     0,    78,    66,     8,    67,
       0,     0,     0,     0,    68,    69,    70,     0,    71,    72,
      73,    74,    75,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    76,     0,    53,   214,     0,     0,    78,
      66,     8,    67,     0,     0,     0,     0,    68,    69,    70,
       0,    71,    72,    73,    74,    75,     0,    66,     8,    67,
       0,     0,     0,     0,    68,    69,    76,     0,    53,    72,
       0,     0,    78,    66,     8,    67,     0,     0,   133,   134,
      68,    69,     0,   135,     0,    72,     0,     0,     0,   148,
      66,     8,    67,     0,   133,   134,     0,    68,    69,   135,
       0,     0,    72,     0,   122,     0,    66,     8,    67,    22,
       0,   133,   134,    68,    69,     0,   135,   209,    72,     0,
       0,     0,    66,     8,    67,     0,     0,   133,   134,    68,
      69,     0,   135,     0,    72,     0,     0,     0,     0,     0,
       0,     0,     0,   133,   134,     0,     0,     0,   135
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-131))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
       2,    73,    74,    75,    36,    72,   136,    29,    43,    41,
      43,    33,    43,    15,    16,    50,    43,    15,    53,    50,
     129,   130,    53,    25,    26,    57,    58,     9,   133,   134,
      63,    30,     4,    35,     6,    70,    71,    36,    31,     4,
      33,    43,     4,    32,    20,    34,    81,    20,    81,    31,
      81,    29,    43,    29,    81,    33,    29,    33,    30,    50,
      33,    63,    53,    73,    74,    75,    76,    29,   135,    58,
      72,    33,    30,     7,    72,    37,   111,   112,    36,    81,
     111,   112,     0,    98,     4,   112,     6,   159,    35,    36,
      81,   163,   164,   223,   129,   130,   201,   202,   129,   130,
       5,     8,   104,    29,   108,   109,    11,    21,    29,   218,
      24,    37,    32,    18,   116,    35,   131,   132,    38,    23,
     111,   112,     4,    27,   126,   135,   128,     4,   132,     6,
      12,   133,   134,   135,   136,     6,   171,     8,   129,   130,
     171,    30,   174,   176,   171,   177,     4,   157,     6,   159,
     182,    31,   184,   163,   164,    32,   158,    31,    35,   191,
     192,    38,   234,   197,   198,   133,   134,   182,   136,   184,
      30,   199,   200,    33,   176,   179,   133,   134,    31,   136,
     171,    31,    26,   218,    28,    33,    29,   218,   220,    29,
     222,    37,    34,    29,   196,   197,   198,   199,   200,   201,
     202,    35,    35,   207,    20,    37,    33,    20,    20,    35,
       3,     4,     5,    30,    29,    35,    22,    10,    11,    30,
      34,   223,    15,    13,   234,    35,    35,   218,   196,   197,
     198,   199,   200,   201,   202,    36,    29,    35,    35,   196,
     197,   198,   199,   200,   201,   202,    33,     5,    20,     3,
       4,     5,     6,    26,   116,   223,    10,    11,    12,   104,
      14,    15,    16,    17,    18,    59,   223,   176,   196,   157,
     111,   132,   184,    81,    -1,    29,   135,    31,    32,    -1,
      -1,    35,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    -1,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,    -1,
      31,    32,    -1,    -1,    35,     3,     4,     5,    -1,    -1,
      -1,    -1,    10,    11,    12,    -1,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    29,    -1,    31,    32,    -1,    -1,    35,     3,     4,
       5,    -1,    -1,    -1,    -1,    10,    11,    12,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    29,    -1,    31,    32,    -1,    -1,
      35,     3,     4,     5,    -1,    -1,    -1,    -1,    10,    11,
      12,    -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,    -1,    31,
      32,    -1,    -1,    35,     3,     4,     5,    -1,    -1,    -1,
      -1,    10,    11,    12,    -1,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      29,    -1,    31,    32,    -1,    -1,    35,     3,     4,     5,
      -1,    -1,    -1,    -1,    10,    11,    12,    -1,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    29,    -1,    31,    32,    -1,    -1,    35,
       3,     4,     5,    -1,    -1,    -1,    -1,    10,    11,    12,
      -1,    14,    15,    16,    17,    18,    -1,     3,     4,     5,
      -1,    -1,    -1,    -1,    10,    11,    29,    -1,    31,    15,
      -1,    -1,    35,     3,     4,     5,    -1,    -1,    24,    25,
      10,    11,    -1,    29,    -1,    15,    -1,    -1,    -1,    35,
       3,     4,     5,    -1,    24,    25,    -1,    10,    11,    29,
      -1,    -1,    15,    -1,    34,    -1,     3,     4,     5,     6,
      -1,    24,    25,    10,    11,    -1,    29,    30,    15,    -1,
      -1,    -1,     3,     4,     5,    -1,    -1,    24,    25,    10,
      11,    -1,    29,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    24,    25,    -1,    -1,    -1,    29
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     6,     8,    40,    41,    43,    44,     7,     4,   115,
       0,    43,    41,    44,    29,     9,    31,    45,    43,    30,
     113,   115,     6,    32,    35,    38,    46,    47,    48,    49,
      52,    55,    56,    59,    60,   108,   109,   110,   111,   112,
     113,   114,   115,    31,    42,    69,    45,   115,    32,    47,
      31,    57,    66,    31,    61,    66,    50,    51,    53,   115,
      33,   101,   101,    29,    62,   101,     3,     5,    10,    11,
      12,    14,    15,    16,    17,    18,    29,    32,    35,    66,
      68,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    86,    87,    94,    95,    96,
      97,   102,   103,   104,   108,   115,   116,    29,    10,    11,
      32,    58,    67,    68,   115,    35,    36,   101,    54,    66,
     101,    62,    34,    30,    63,    64,   108,   115,    37,    81,
      81,   109,   113,    24,    25,    29,    81,    84,    85,    86,
      88,    89,    90,    91,    92,    93,    94,   103,    35,    84,
      84,    84,    32,    71,    35,    35,    20,   107,    37,    33,
      99,    50,   115,    29,    33,    99,   105,    30,   105,   105,
      32,    67,    32,    68,    51,    30,    36,    65,   115,   115,
      74,    74,    98,    99,    98,   105,    92,   102,   104,   115,
      92,   109,   114,   115,    93,    35,    22,    23,    27,    21,
      24,    26,    28,    35,    35,    30,    85,   115,    84,    30,
      84,   106,    35,    35,    32,    64,   101,   105,    13,    99,
     100,   101,   100,    30,    89,    90,    90,    91,    91,    92,
      92,   105,    34,    30,    36,    74,   101,    93,    84
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* This macro is provided for backward compatibility. */

#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  YYSIZE_T yysize1;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = 0;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                yysize1 = yysize + yytnamerr (0, yytname[yyx]);
                if (! (yysize <= yysize1
                       && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                  return 2;
                yysize = yysize1;
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  yysize1 = yysize + yystrlen (yyformat);
  if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
    return 2;
  yysize = yysize1;

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1806 of yacc.c  */
#line 138 "parse.y"
    {}
    break;

  case 3:

/* Line 1806 of yacc.c  */
#line 140 "parse.y"
    {
	  (yyval.y_node) = makeCUnit(makeSeq((yyvsp[(1) - (1)].y_node)));
	}
    break;

  case 4:

/* Line 1806 of yacc.c  */
#line 144 "parse.y"
    {
	  (yyval.y_node) = makeCUnit(appendSeq((yyvsp[(2) - (2)].y_node), makeSeq((yyvsp[(1) - (2)].y_node))));
	}
    break;

  case 5:

/* Line 1806 of yacc.c  */
#line 148 "parse.y"
    {
	  (yyval.y_node) = makeCUnit(appendSeq((yyvsp[(1) - (2)].y_node), makeSeq((yyvsp[(2) - (2)].y_node))));
	}
    break;

  case 6:

/* Line 1806 of yacc.c  */
#line 152 "parse.y"
    {
	  (yyval.y_node) = makeCUnit(appendSeq((yyvsp[(3) - (3)].y_node), appendSeq((yyvsp[(1) - (3)].y_node), makeSeq((yyvsp[(2) - (3)].y_node)))));
	}
    break;

  case 7:

/* Line 1806 of yacc.c  */
#line 159 "parse.y"
    {
		(yyval.y_node) = makeMainFunc((yyvsp[(5) - (5)].y_node), (yyvsp[(2) - (5)].y_line));
	}
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 166 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(1) - (1)].y_node);
	}
    break;

  case 9:

/* Line 1806 of yacc.c  */
#line 173 "parse.y"
    {
		(yyval.y_node) = appendSeq(makeSeq((yyvsp[(2) - (2)].y_node)), (yyvsp[(1) - (2)].y_node));
	}
    break;

  case 10:

/* Line 1806 of yacc.c  */
#line 177 "parse.y"
    {
		(yyval.y_node) = makeSeq((yyvsp[(1) - (1)].y_node));
	}
    break;

  case 11:

/* Line 1806 of yacc.c  */
#line 184 "parse.y"
    {
		(yyval.y_node) = makeClass(makeNameId((yyvsp[(2) - (3)].y_literal)), NULL, (yyvsp[(3) - (3)].y_node), (yyvsp[(1) - (3)].y_line));
		free((yyvsp[(2) - (3)].y_literal));
	}
    break;

  case 12:

/* Line 1806 of yacc.c  */
#line 189 "parse.y"
    {
		(yyval.y_node) = makeClass(makeNameId((yyvsp[(2) - (5)].y_literal)), (yyvsp[(4) - (5)].y_node), (yyvsp[(5) - (5)].y_node), (yyvsp[(1) - (5)].y_line));
		free((yyvsp[(2) - (5)].y_literal));
	}
    break;

  case 13:

/* Line 1806 of yacc.c  */
#line 197 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(2) - (3)].y_node);
	}
    break;

  case 14:

/* Line 1806 of yacc.c  */
#line 201 "parse.y"
    {
		(yyval.y_node) = NULL;
	}
    break;

  case 15:

/* Line 1806 of yacc.c  */
#line 208 "parse.y"
    {
		(yyval.y_node) = appendSeq(makeSeq((yyvsp[(2) - (2)].y_node)), (yyvsp[(1) - (2)].y_node));
	}
    break;

  case 16:

/* Line 1806 of yacc.c  */
#line 212 "parse.y"
    {
		(yyval.y_node) = makeSeq((yyvsp[(1) - (1)].y_node));
	}
    break;

  case 17:

/* Line 1806 of yacc.c  */
#line 219 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(1) - (1)].y_node);
	}
    break;

  case 18:

/* Line 1806 of yacc.c  */
#line 223 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(1) - (1)].y_node);
	}
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 227 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(1) - (1)].y_node);
	}
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 231 "parse.y"
    {
		(yyval.y_node) = NULL;
	}
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 238 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(1) - (1)].y_node);
	}
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 242 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(1) - (1)].y_node);
	}
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 249 "parse.y"
    {
		(yyval.y_node) = makeFieldDecl((yyvsp[(1) - (3)].y_node), (yyvsp[(2) - (3)].y_node), (yyvsp[(3) - (3)].y_line));
	}
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 256 "parse.y"
    {
		(yyval.y_node) = appendSeq(makeSeq((yyvsp[(3) - (3)].y_node)), (yyvsp[(1) - (3)].y_node));	
	}
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 260 "parse.y"
    {
		(yyval.y_node) = makeSeq((yyvsp[(1) - (1)].y_node));	
	}
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 267 "parse.y"
    {
		(yyval.y_node) = makeArray((yyvsp[(1) - (2)].y_node), (yyvsp[(1) - (2)].y_node)->line);
	}
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 271 "parse.y"
    {
		(yyval.y_node) = makeNameId((yyvsp[(1) - (1)].y_literal));
		free((yyvsp[(1) - (1)].y_literal));
	}
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 279 "parse.y"
    {
		(yyval.y_node) = makeMethDecl((yyvsp[(1) - (3)].y_node), (yyvsp[(2) - (3)].y_node), (yyvsp[(3) - (3)].y_node), (yyvsp[(1) - (3)].y_node)->line);
	}
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 286 "parse.y"
    {
		(yyval.y_node) = makeMethDeclator(makeNameId((yyvsp[(1) - (2)].y_literal)), (yyvsp[(2) - (2)].y_node), (yyvsp[(1) - (2)].y_literal)->line);
		free((yyvsp[(1) - (2)].y_literal));
	}
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 291 "parse.y"
    {
		(yyval.y_node) = makeArray((yyvsp[(1) - (2)].y_node), (yyvsp[(1) - (2)].y_node)->line);
	}
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 298 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(1) - (1)].y_node);
	}
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 305 "parse.y"
    {
		(yyval.y_node) = makeConstDecl((yyvsp[(1) - (2)].y_node), (yyvsp[(2) - (2)].y_node), (yyvsp[(1) - (2)].y_node)->line);
	}
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 312 "parse.y"
    {
		(yyval.y_node) = makeConstDeclator(makeNameId((yyvsp[(1) - (2)].y_literal)), (yyvsp[(2) - (2)].y_node), (yyvsp[(1) - (2)].y_literal)->line);
		free((yyvsp[(1) - (2)].y_literal));
	}
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 320 "parse.y"
    {
		(yyval.y_node) = appendSeq((yyvsp[(3) - (4)].y_node), makeSeq((yyvsp[(2) - (4)].y_node)));
	}
    break;

  case 35:

/* Line 1806 of yacc.c  */
#line 324 "parse.y"
    {
		(yyval.y_node) = makeSeq((yyvsp[(2) - (3)].y_node));
	}
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 328 "parse.y"
    {
	  (yyval.y_node) = (yyvsp[(1) - (1)].y_node);
	}
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 335 "parse.y"
    {
		(yyval.y_node) = makeConstInvoc(makeThis((yyvsp[(1) - (3)].y_line)), (yyvsp[(2) - (3)].y_node), (yyvsp[(1) - (3)].y_line));
	}
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 339 "parse.y"
    {
		(yyval.y_node) = makeConstInvoc(makeSuper((yyvsp[(1) - (3)].y_line)), (yyvsp[(2) - (3)].y_node), (yyvsp[(1) - (3)].y_line));
	}
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 346 "parse.y"
    {
		(yyval.y_node) = makeDestructor((yyvsp[(1) - (2)].y_node), (yyvsp[(2) - (2)].y_node), (yyvsp[(1) - (2)].y_node)->line);
	}
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 353 "parse.y"
    {
		(yyval.y_node) = makeNameId((yyvsp[(2) - (4)].y_literal));
		free((yyvsp[(2) - (4)].y_literal));
	}
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 361 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(1) - (1)].y_node);
	}
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 368 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(2) - (3)].y_node);
	}
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 372 "parse.y"
    {
		(yyval.y_node) = NULL;
	}
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 379 "parse.y"
    {
		(yyval.y_node) = appendSeq(makeSeq((yyvsp[(3) - (3)].y_node)), (yyvsp[(1) - (3)].y_node));
	}
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 383 "parse.y"
    {
		(yyval.y_node) = makeSeq((yyvsp[(1) - (1)].y_node));
	}
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 390 "parse.y"
    {
		(yyval.y_node) = makeFormalParam((yyvsp[(1) - (2)].y_node), (yyvsp[(2) - (2)].y_node), (yyvsp[(1) - (2)].y_node)->line);
	}
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 397 "parse.y"
    {
		(yyval.y_node) = makeArray((yyvsp[(1) - (2)].y_node), (yyvsp[(1) - (2)].y_node)->line);
	}
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 401 "parse.y"
    {
		(yyval.y_node) = makeNameId((yyvsp[(1) - (1)].y_literal));
		free((yyvsp[(1) - (1)].y_literal));
	}
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 409 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(2) - (3)].y_node);
	}
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 413 "parse.y"
    {
		(yyval.y_node) = NULL;
	}
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 420 "parse.y"
    {
		(yyval.y_node) = appendSeq(makeSeq((yyvsp[(2) - (2)].y_node)), (yyvsp[(1) - (2)].y_node));
	}
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 424 "parse.y"
    {
		(yyval.y_node) = makeSeq((yyvsp[(1) - (1)].y_node));
	}
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 431 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(1) - (1)].y_node);
	}
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 438 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(2) - (3)].y_node);
	}
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 442 "parse.y"
    {
		(yyval.y_node) = NULL;
	}
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 449 "parse.y"
    {
		(yyval.y_node) = appendSeq(makeSeq((yyvsp[(2) - (2)].y_node)), (yyvsp[(1) - (2)].y_node));
	}
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 453 "parse.y"
    {
		(yyval.y_node) = makeSeq((yyvsp[(1) - (1)].y_node));
	}
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 460 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(1) - (1)].y_node);
	}
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 464 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(1) - (1)].y_node);
	}
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 471 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(1) - (2)].y_node);
	}
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 478 "parse.y"
    {
		(yyval.y_node) = makeMainVarDecl((yyvsp[(1) - (2)].y_node), (yyvsp[(2) - (2)].y_node), (yyvsp[(1) - (2)].y_node)->line);
	}
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 485 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(1) - (1)].y_node);
	}
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 489 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(1) - (1)].y_node);
	}
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 493 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(1) - (1)].y_node);
	}
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 497 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(1) - (1)].y_node);
	}
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 501 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(1) - (1)].y_node);
	}
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 505 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(1) - (1)].y_node);
	}
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 509 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(1) - (1)].y_node);
	}
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 513 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(1) - (1)].y_node);
	}
    break;

  case 70:

/* Line 1806 of yacc.c  */
#line 520 "parse.y"
    {
		(yyval.y_node) = makeIfElseSt((yyvsp[(2) - (5)].y_node), (yyvsp[(3) - (5)].y_node), (yyvsp[(5) - (5)].y_node), (yyvsp[(1) - (5)].y_line));
	}
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 527 "parse.y"
    {
		(yyval.y_node) = makeWhileSt((yyvsp[(2) - (3)].y_node), (yyvsp[(3) - (3)].y_node), (yyvsp[(1) - (3)].y_line));
	}
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 534 "parse.y"
    {
		(yyval.y_node) = makeReturnSt(NULL, (yyvsp[(1) - (2)].y_line));
	}
    break;

  case 73:

/* Line 1806 of yacc.c  */
#line 538 "parse.y"
    {
		(yyval.y_node) = makeReturnSt((yyvsp[(2) - (3)].y_node), (yyvsp[(1) - (3)].y_line));
	}
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 545 "parse.y"
    {
		(yyval.y_node) = makeDeleteSt((yyvsp[(2) - (3)].y_node), (yyvsp[(1) - (3)].y_line));
	}
    break;

  case 75:

/* Line 1806 of yacc.c  */
#line 552 "parse.y"
    {
		(yyval.y_node) = makeOutSt((yyvsp[(2) - (3)].y_node), (yyvsp[(1) - (3)].y_line));
	}
    break;

  case 76:

/* Line 1806 of yacc.c  */
#line 559 "parse.y"
    {
		(yyval.y_node) = NULL;
	}
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 566 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(2) - (3)].y_node);
	}
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 573 "parse.y"
    {
		(yyval.y_node) = makeExprSt((yyvsp[(1) - (2)].y_node));
	}
    break;

  case 79:

/* Line 1806 of yacc.c  */
#line 580 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(1) - (1)].y_node);
	}
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 584 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(1) - (1)].y_node);
	}
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 591 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(1) - (1)].y_node);
	}
    break;

  case 82:

/* Line 1806 of yacc.c  */
#line 598 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(1) - (1)].y_node);
	}
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 602 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(1) - (1)].y_node);
	}
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 609 "parse.y"
    {
		(yyval.y_node) = makeBinaryOp(ASI_OP, (yyvsp[(1) - (3)].y_node), (yyvsp[(3) - (3)].y_node), (yyvsp[(1) - (3)].y_node)->line);
	}
    break;

  case 85:

/* Line 1806 of yacc.c  */
#line 616 "parse.y"
    {
		(yyval.y_node) = makeExprId((yyvsp[(1) - (1)].y_literal));
		free((yyvsp[(1) - (1)].y_literal));
	}
    break;

  case 86:

/* Line 1806 of yacc.c  */
#line 621 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(1) - (1)].y_node);
	}
    break;

  case 87:

/* Line 1806 of yacc.c  */
#line 625 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(1) - (1)].y_node);
	}
    break;

  case 88:

/* Line 1806 of yacc.c  */
#line 632 "parse.y"
    {
		(yyval.y_node) = makeBinaryOp(EQU_OP, (yyvsp[(1) - (3)].y_node), (yyvsp[(3) - (3)].y_node), (yyvsp[(2) - (3)].y_line));
	}
    break;

  case 89:

/* Line 1806 of yacc.c  */
#line 636 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(1) - (1)].y_node);
	}
    break;

  case 90:

/* Line 1806 of yacc.c  */
#line 643 "parse.y"
    {
		(yyval.y_node) = makeBinaryOp(LES_OP, (yyvsp[(1) - (3)].y_node), (yyvsp[(3) - (3)].y_node), (yyvsp[(2) - (3)].y_line));
	}
    break;

  case 91:

/* Line 1806 of yacc.c  */
#line 647 "parse.y"
    {
		(yyval.y_node) = makeBinaryOp(GRT_OP, (yyvsp[(1) - (3)].y_node), (yyvsp[(3) - (3)].y_node), (yyvsp[(2) - (3)].y_line));
	}
    break;

  case 92:

/* Line 1806 of yacc.c  */
#line 651 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(1) - (1)].y_node);
	}
    break;

  case 93:

/* Line 1806 of yacc.c  */
#line 658 "parse.y"
    {
		(yyval.y_node) = makeBinaryOp(ADD_OP, (yyvsp[(1) - (3)].y_node), (yyvsp[(3) - (3)].y_node), (yyvsp[(2) - (3)].y_line));
	}
    break;

  case 94:

/* Line 1806 of yacc.c  */
#line 662 "parse.y"
    {
		(yyval.y_node) = makeBinaryOp(SUB_OP, (yyvsp[(1) - (3)].y_node), (yyvsp[(3) - (3)].y_node), (yyvsp[(2) - (3)].y_line));
	}
    break;

  case 95:

/* Line 1806 of yacc.c  */
#line 666 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(1) - (1)].y_node);
	}
    break;

  case 96:

/* Line 1806 of yacc.c  */
#line 673 "parse.y"
    {
		(yyval.y_node) = makeBinaryOp(MUL_OP, (yyvsp[(1) - (3)].y_node), (yyvsp[(3) - (3)].y_node), (yyvsp[(2) - (3)].y_line));
	}
    break;

  case 97:

/* Line 1806 of yacc.c  */
#line 677 "parse.y"
    {
		(yyval.y_node) = makeBinaryOp(DIV_OP, (yyvsp[(1) - (3)].y_node), (yyvsp[(3) - (3)].y_node), (yyvsp[(2) - (3)].y_line));
	}
    break;

  case 98:

/* Line 1806 of yacc.c  */
#line 681 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(1) - (1)].y_node);
	}
    break;

  case 99:

/* Line 1806 of yacc.c  */
#line 688 "parse.y"
    {
		(yyval.y_node) = makeUnaryOp(NEG_OP, (yyvsp[(2) - (2)].y_node), (yyvsp[(1) - (2)].y_line));
	}
    break;

  case 100:

/* Line 1806 of yacc.c  */
#line 692 "parse.y"
    {
		(yyval.y_node) = makeUnaryOp(NOT_OP, (yyvsp[(2) - (2)].y_node), (yyvsp[(1) - (2)].y_line));
	}
    break;

  case 101:

/* Line 1806 of yacc.c  */
#line 696 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(1) - (1)].y_node);
	}
    break;

  case 102:

/* Line 1806 of yacc.c  */
#line 703 "parse.y"
    {
		(yyval.y_node) = makeCast((yyvsp[(1) - (2)].y_node), (yyvsp[(2) - (2)].y_node), (yyvsp[(1) - (2)].y_node)->line);
	}
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 707 "parse.y"
    {
		(yyval.y_node) = makeCast((yyvsp[(2) - (4)].y_node), (yyvsp[(4) - (4)].y_node), (yyvsp[(1) - (4)].y_line));
	}
    break;

  case 104:

/* Line 1806 of yacc.c  */
#line 711 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(1) - (1)].y_node);
	}
    break;

  case 105:

/* Line 1806 of yacc.c  */
#line 718 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(1) - (1)].y_node);
	}
    break;

  case 106:

/* Line 1806 of yacc.c  */
#line 722 "parse.y"
    {
		(yyval.y_node) = makeExprId((yyvsp[(1) - (1)].y_literal));
		free((yyvsp[(1) - (1)].y_literal));
	}
    break;

  case 107:

/* Line 1806 of yacc.c  */
#line 727 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(1) - (1)].y_node);
	}
    break;

  case 108:

/* Line 1806 of yacc.c  */
#line 734 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(1) - (1)].y_node);
	}
    break;

  case 109:

/* Line 1806 of yacc.c  */
#line 738 "parse.y"
    {
		(yyval.y_node) = makeThis((yyvsp[(1) - (1)].y_line));
	}
    break;

  case 110:

/* Line 1806 of yacc.c  */
#line 742 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(1) - (1)].y_node);
	}
    break;

  case 111:

/* Line 1806 of yacc.c  */
#line 746 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(1) - (1)].y_node);
	}
    break;

  case 112:

/* Line 1806 of yacc.c  */
#line 750 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(1) - (1)].y_node);
	}
    break;

  case 113:

/* Line 1806 of yacc.c  */
#line 754 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(1) - (1)].y_node);
	}
    break;

  case 114:

/* Line 1806 of yacc.c  */
#line 758 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(1) - (1)].y_node);
	}
    break;

  case 115:

/* Line 1806 of yacc.c  */
#line 765 "parse.y"
    {
		(yyval.y_node) = makeClassCreate((yyvsp[(2) - (3)].y_node), (yyvsp[(3) - (3)].y_node), (yyvsp[(1) - (3)].y_line));
	}
    break;

  case 116:

/* Line 1806 of yacc.c  */
#line 772 "parse.y"
    {
		(yyval.y_node) = makeArrayCreate((yyvsp[(2) - (4)].y_node), (yyvsp[(3) - (4)].y_node), (yyvsp[(4) - (4)].y_int), (yyvsp[(1) - (4)].y_line));
	}
    break;

  case 117:

/* Line 1806 of yacc.c  */
#line 776 "parse.y"
    {
		(yyval.y_node) = makeArrayCreate((yyvsp[(2) - (3)].y_node), (yyvsp[(3) - (3)].y_node), 0, (yyvsp[(1) - (3)].y_line));
	}
    break;

  case 118:

/* Line 1806 of yacc.c  */
#line 780 "parse.y"
    {
		(yyval.y_node) = makeArrayCreate((yyvsp[(2) - (4)].y_node), (yyvsp[(3) - (4)].y_node), (yyvsp[(4) - (4)].y_int), (yyvsp[(1) - (4)].y_line));
	}
    break;

  case 119:

/* Line 1806 of yacc.c  */
#line 784 "parse.y"
    {
		(yyval.y_node) = makeArrayCreate((yyvsp[(2) - (3)].y_node), (yyvsp[(3) - (3)].y_node), 0, (yyvsp[(1) - (3)].y_line));
	}
    break;

  case 120:

/* Line 1806 of yacc.c  */
#line 791 "parse.y"
    {
		(yyval.y_node) = appendSeq(makeSeq((yyvsp[(2) - (2)].y_node)), (yyvsp[(1) - (2)].y_node));
	}
    break;

  case 121:

/* Line 1806 of yacc.c  */
#line 795 "parse.y"
    {
		(yyval.y_node) = makeSeq((yyvsp[(1) - (1)].y_node));
	}
    break;

  case 122:

/* Line 1806 of yacc.c  */
#line 802 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(2) - (3)].y_node);
	}
    break;

  case 123:

/* Line 1806 of yacc.c  */
#line 809 "parse.y"
    {
		(yyval.y_int) = (yyvsp[(1) - (2)].y_int) + 1;
	}
    break;

  case 124:

/* Line 1806 of yacc.c  */
#line 813 "parse.y"
    {
		(yyval.y_int) = 1;
	}
    break;

  case 125:

/* Line 1806 of yacc.c  */
#line 820 "parse.y"
    {}
    break;

  case 126:

/* Line 1806 of yacc.c  */
#line 825 "parse.y"
    {
		(yyval.y_node) = makeFieldAccess((yyvsp[(1) - (3)].y_node), makeNameId((yyvsp[(3) - (3)].y_literal)), (yyvsp[(2) - (3)].y_line));
		free((yyvsp[(3) - (3)].y_literal));
	}
    break;

  case 127:

/* Line 1806 of yacc.c  */
#line 830 "parse.y"
    {
		(yyval.y_node) = makeFieldAccess(makeSuper((yyvsp[(1) - (3)].y_line)), makeNameId((yyvsp[(3) - (3)].y_literal)), (yyvsp[(2) - (3)].y_line));
		free((yyvsp[(3) - (3)].y_literal));
	}
    break;

  case 128:

/* Line 1806 of yacc.c  */
#line 838 "parse.y"
    {
		(yyval.y_node) = makeMethodInvoc(NULL, makeNameId((yyvsp[(1) - (2)].y_literal)), (yyvsp[(2) - (2)].y_node), (yyvsp[(1) - (2)].y_literal)->line);
		free((yyvsp[(1) - (2)].y_literal));
	}
    break;

  case 129:

/* Line 1806 of yacc.c  */
#line 843 "parse.y"
    {
		(yyval.y_node) = makeMethodInvoc((yyvsp[(1) - (4)].y_node), makeNameId((yyvsp[(3) - (4)].y_literal)), (yyvsp[(4) - (4)].y_node), (yyvsp[(2) - (4)].y_line));
		free((yyvsp[(3) - (4)].y_literal));
	}
    break;

  case 130:

/* Line 1806 of yacc.c  */
#line 848 "parse.y"
    {
		(yyval.y_node) = makeMethodInvoc(makeSuper((yyvsp[(1) - (4)].y_line)), makeNameId((yyvsp[(3) - (4)].y_literal)), (yyvsp[(4) - (4)].y_node), (yyvsp[(1) - (4)].y_line));
		free((yyvsp[(3) - (4)].y_literal));
	}
    break;

  case 131:

/* Line 1806 of yacc.c  */
#line 856 "parse.y"
    {
		(yyval.y_node) = makeArrayAccess(makeExprId((yyvsp[(1) - (2)].y_literal)), (yyvsp[(2) - (2)].y_node), (yyvsp[(1) - (2)].y_literal)->line);
		free((yyvsp[(1) - (2)].y_literal));
	}
    break;

  case 132:

/* Line 1806 of yacc.c  */
#line 861 "parse.y"
    {
		(yyval.y_node) = makeArrayAccess((yyvsp[(1) - (2)].y_node), (yyvsp[(2) - (2)].y_node), (yyvsp[(1) - (2)].y_node)->line);
	}
    break;

  case 133:

/* Line 1806 of yacc.c  */
#line 868 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(2) - (3)].y_node);
	}
    break;

  case 134:

/* Line 1806 of yacc.c  */
#line 872 "parse.y"
    {
		(yyval.y_node) = NULL;
	}
    break;

  case 135:

/* Line 1806 of yacc.c  */
#line 879 "parse.y"
    {
		(yyval.y_node) = appendSeq(makeSeq((yyvsp[(3) - (3)].y_node)), (yyvsp[(1) - (3)].y_node));
	}
    break;

  case 136:

/* Line 1806 of yacc.c  */
#line 883 "parse.y"
    {
		(yyval.y_node) = makeSeq((yyvsp[(1) - (1)].y_node));
	}
    break;

  case 137:

/* Line 1806 of yacc.c  */
#line 890 "parse.y"
    {}
    break;

  case 138:

/* Line 1806 of yacc.c  */
#line 895 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(1) - (1)].y_node);
	}
    break;

  case 139:

/* Line 1806 of yacc.c  */
#line 899 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(1) - (1)].y_node);
	}
    break;

  case 140:

/* Line 1806 of yacc.c  */
#line 906 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(1) - (1)].y_node);
	}
    break;

  case 141:

/* Line 1806 of yacc.c  */
#line 913 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(1) - (1)].y_node);
	}
    break;

  case 142:

/* Line 1806 of yacc.c  */
#line 920 "parse.y"
    {
		(yyval.y_node) = makeInt((yyvsp[(1) - (1)].y_line));
	}
    break;

  case 143:

/* Line 1806 of yacc.c  */
#line 927 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(1) - (1)].y_node);
	}
    break;

  case 144:

/* Line 1806 of yacc.c  */
#line 931 "parse.y"
    {
		(yyval.y_node) = (yyvsp[(1) - (1)].y_node);
	}
    break;

  case 145:

/* Line 1806 of yacc.c  */
#line 938 "parse.y"
    {
		(yyval.y_node) = makeNameId((yyvsp[(1) - (1)].y_literal));
		free((yyvsp[(1) - (1)].y_literal));
	}
    break;

  case 146:

/* Line 1806 of yacc.c  */
#line 946 "parse.y"
    {
		(yyval.y_node) = makeArray((yyvsp[(1) - (2)].y_node), (yyvsp[(1) - (2)].y_node)->line);
	}
    break;

  case 147:

/* Line 1806 of yacc.c  */
#line 950 "parse.y"
    {
		(yyval.y_node) = makeArray(makeNameId((yyvsp[(1) - (2)].y_literal)), (yyvsp[(1) - (2)].y_literal)->line);
		free((yyvsp[(1) - (2)].y_literal));
	}
    break;

  case 148:

/* Line 1806 of yacc.c  */
#line 955 "parse.y"
    {
		(yyval.y_node) = makeArray((yyvsp[(1) - (2)].y_node), (yyvsp[(1) - (2)].y_node)->line);
	}
    break;

  case 149:

/* Line 1806 of yacc.c  */
#line 962 "parse.y"
    {
		(yyval.y_literal) = (yyvsp[(1) - (1)].y_literal);
	}
    break;

  case 150:

/* Line 1806 of yacc.c  */
#line 969 "parse.y"
    {
		(yyval.y_node) = makeIntLit((yyvsp[(1) - (1)].y_literal));
	}
    break;

  case 151:

/* Line 1806 of yacc.c  */
#line 973 "parse.y"
    {
		(yyval.y_node) = makeNullLit((yyvsp[(1) - (1)].y_line));
	}
    break;



/* Line 1806 of yacc.c  */
#line 3091 "y.tab.c"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 2067 of yacc.c  */
#line 978 "parse.y"


void yyerror(const char *s)
{
  extern unsigned int line; /* scan.l */
  
  fprintf(stderr, "Line %d: %s\n", line, s);
  t_free_alld();

  exit(1);
}

