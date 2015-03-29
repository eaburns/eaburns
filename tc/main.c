/*
 * main.c contains the main function and helper functions.
 * Ethan Burns -- Created Wed Feb 15 10:19:20 EST 2006
 */
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "ast.h"
#include "class.h"
#include "type.h"
#include "analyze.h"
#include "error.h"
#include "codegen.h"

#define TMP_FILE "/tmp/t.o"

static FILE *parseArgs(int argc, char *argv[]);

int
main(int argc, char *argv[])
{
  /*
   * Some things defined in parse.y. 
   */
  extern FILE *yyin;
  extern void yyparse();

  FILE * outfile = stdin;

  yyin = parseArgs(argc, argv);

  initAst();

  yyparse();

  if (ast != NULL) {
    initTypes();                  /* Initialize type memory tracking. */
    buildClasses(ast);

    /* Run symantic checking. */
    analyze(ast);

    /* If there was errors, return the number. */
    if (error_count > 0) {
      return error_count;
    }

#if 0
		printNode(ast);
		outfile = stdin; /* to mark it used... __attribute__((used)) doesn't work */
#else
    output_code(ast, outfile);
#endif
  }

  freeTypes();                  /* Free type memory. */
  freeAst();                    /* Free the AST memory. */

  return 0;
}

/*
 * Parse command line arguments, and return a pointer to the input file. 
 */
static FILE *
parseArgs(int argc, char *argv[])
{
  FILE *f;

  if (argc == 1)
    return stdin;

  /*
   * Last argument is the file. 
   */
  f = fopen(argv[argc - 1], "r");

  if (f == NULL) {
    fprintf(stderr, "Unable to open input file %s: ", argv[argc - 1]);
    perror("");
    exit(1);
  }

  return f;
}
