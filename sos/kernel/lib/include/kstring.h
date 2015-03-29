#include <ktypes.h>

/* Returns -1 if a<b, 1 if b<a or 0 if a==b */
int kstrcmp( const char *a, const char *b );

/* Returns the length of a null terminated string. */
uint32 kstrlen( const char *a );

/* Concatinates two strings, with the resultant string being in the 
 * first argument.  The return value is a pointer to the resulting 
 * string. */
char *kstrcat( char *dest, const char *src );

/* Copies src to dest, and returns a pointer to dest. */
char *kstrcpy( char *dest, const char *src );
