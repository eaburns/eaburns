/*
 * literal.h defines the Literal type which is a string or an int with a line
 * number that it was parsed on.
 * Ethan Burns -- Created Wed Feb 22 17:12:43 EST 2006
 */
#ifndef _LITERAL_H_
#define _LITERAL_H_

/*
 * A Literal is an integer or a string and which line number it was parsed 
 * on. 
 */
typedef struct {
  char *str;
  unsigned int line;

} Literal;

/*
 * Makes a literal.
 */
Literal *makeLiteral(char *str, unsigned int line);

#endif                          /* !_LITERAL_H_ */
