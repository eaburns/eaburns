#include <kstring.h>
#include <ktypes.h>
#include <memutil.h>

int kstrcmp( const char *a, const char *b )
{
  for( ; *a != '\0'; a++, b++ ) {
    if( *a < *b ) { return -1; }
    if( *b < *a ) { return 1; }
  }

  return 0;
}

uint32 kstrlen( const char *a )
{
  char *p;
  uint32 count = 0;

  for( p = (char*)a; *p != '\0'; count++, p++ );

  return count;
}

char *kstrcat( char *dest, const char *src )
{
  char *p, *q;
  for( p = dest; *p != '\0'; p++ );

  for( q = (char*)src; *q != '\0'; p++, q++ )
    *p = *q;

  *p = '\0';
  
  return dest;
}

char *kstrcpy( char *dest, const char *src )
{
  uint32 len = kstrlen(src);

  memcpy( dest, src, len );

  dest[len] = '\0';

  return dest;
}
