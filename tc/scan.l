%{
/*
 * scan.l defines the lex scanner for the T programming language.
 * Ethan Burns -- Created <unknown date>
 * Ethan Burns -- Added better line numbering support 
 *                Wed Feb 22 17:13:31 EST 2006
 */
#include <string.h> /* strdup.h */
#include "ast.h"	/* MUST come before y.tab.h */
#include "literal.h"	/* MUST come before y.tab.h */
#include "y.tab.h"

#define YY_NO_UNPUT

unsigned int line = 1;

#ifdef DEBUG

# include <stdio.h>
# define token(x) (display_token(#x), x)
YYSTYPE yylval;
int display_token(char *);
# define fatal(x) (fprintf(stderr, x), exit(-1))

#else /* DEBUG */
# define token(x) (x)
#endif /* DEBUG */

%}

letter						([a-zA-Z_])

digit							([0-9])

identifier				({letter}({letter}|{digit})*)

integer_literal		({digit}+)

newline						([\r\n]|("\r\n"))

whitespace				([ \t\f])

comment						("//"[^\r\n]*{newline})

%%

"class"						{ 
										yylval.y_line = line;
										return token(CLASS); 
									}

"delete"					{ 
										yylval.y_line = line;
										return token(DELETE); 
									}

"else"						{ 
										yylval.y_line = line;
										return token(ELSE); 
									}

"extends"					{ 
										yylval.y_line = line;
										return token(EXTENDS); 
									}

"if"							{ 
										yylval.y_line = line;
										return token(IF); 
									}

"int"							{ 
										yylval.y_line = line;
										return token(INT); 
									}

"main"						{ 
										yylval.y_line = line;
										return token(MAIN); 
									}

"new"							{ 
										yylval.y_line = line;
										return token(NEW); 
									}

"out"							{ 
										yylval.y_line = line;
										return token(OUT); 
									}

"return"					{ 
										yylval.y_line = line;
										return token(RETURN); 
									}

"super"						{ 
										yylval.y_line = line;
										return token(SUPER); 
									}

"this"						{ 
										yylval.y_line = line;
										return token(THIS); 
									}

"while"						{ 
										yylval.y_line = line;
										return token(WHILE); 
									}

"null"						{ 
										yylval.y_line = line;
										return token(NULL_LITERAL); 
									}

"="								{ 
										yylval.y_line = line;	
										return token('='); }

"=="							{ 
										yylval.y_line = line;	
										return token(EQ_OP); 
									}
										
"+"								{ 
										yylval.y_line = line;	
										return token('+'); 
									}

">"								{ 
										yylval.y_line = line;	
										return token('>'); 
									}

"-"								{ 
										yylval.y_line = line;	
										return token('-'); 
									}

"!"								{ 
										yylval.y_line = line;	
										return token('!'); 
									}

"/"								{ 
										yylval.y_line = line;	
										return token('/'); 
									}

"<"								{ 
										yylval.y_line = line;	
										return token('<'); 
									}

"*"								{ 
										yylval.y_line = line;	
										return token('*'); 
									}

"("								{ 
										yylval.y_line = line;	
										return token('('); 
									}

")"								{ 
										yylval.y_line = line;	
										return token(')'); 
									}

"{"								{ 
										yylval.y_line = line;	
										return token('{'); 
									}

"}"								{ 
										yylval.y_line = line;	
										return token('}'); 
									}

"["								{ 
										yylval.y_line = line;	
										return token('['); 
									}

"]"								{ 
										yylval.y_line = line;	
										return token(']'); 
									}

";"								{ 
										yylval.y_line = line;	
										return token(';'); 
									}

","								{ 
										yylval.y_line = line;	
										return token(','); 
									}

"."								{ 
										yylval.y_line = line;	
										return token('.'); 
									}

"~"								{ 
										yylval.y_line = line;	
										return token('~'); 
									}

{integer_literal}	{ 
										yylval.y_literal = makeLiteral((char*)strdup(yytext), line);
										return token(INTEGER_LITERAL);
									}


{identifier}			{
										yylval.y_literal = makeLiteral((char*)strdup(yytext), line);
										return token(IDENTIFIER);
									}

{newline}					{ line++; }

{comment}					{ line++; /* Comment has a newline. */ } 

{whitespace}+			{}

.									{ 
										yylval.y_line = line;
										return token(UNKNOWN); 
									}

%%
