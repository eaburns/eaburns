#include <kstdlib.h>
#include <kstdio.h>
#include <ktypes.h>
#include <console.h>

#define ITOA_BUFF_LEN 20  /* Number of digits in 2^32 base 10. */

#define ITOA_NULL 0x0
#define ITOA_UPPERCASE 0x1
#define ITOA_LOWERCASE 0x2

/* Prints an integer of the given base. */
static void itoa( uint32 i, uint32 base, uint8 flags )
{
  uint32 tmp;
  int count = 0;
  char reverse_buff[ITOA_BUFF_LEN];

  /* Get characters from the number and buffer them
   * in reverse order. */
  do {

    /* This should be fixed to use the stack not some stupid array. */
    if( count >= ITOA_BUFF_LEN ) { 
      puts( " itoa buff overflow " );
      break; 
    }

    tmp = i % base;
    i /= base;
    tmp += '0';

    /* Make digits above 9 uppercase or lowercase letters. */ 
    if( tmp > '9' && (flags & ITOA_UPPERCASE) != 0 ) { 
      tmp += 7;
    } else if( tmp > '9' && (flags & ITOA_LOWERCASE) != 0 ) { 
      tmp += 39; 
    }

    reverse_buff[count++] = tmp;
  } while( i != 0 );
  /* Reverse the characters and place them in the return
   * buffer. */
  count--;
  while( count >= 0 )
   putchar( reverse_buff[count--] );
  
}

void puts( char *s )
{
  while( *s ) putchar( *(s++) );
}

#include <config/kern.h>
void kprintf( char *format, ... )
{
  char **argument = &format;
  char *p = format;

  argument++;

  while( *p ) {

    /* Escape character. */
    if( *p == '%' ) {

      p++;
      switch( *p ) {

      /* Print a character. */
      case 'c':
        putchar( (int)(*(argument++)) );
        break;

      /* Print a string. */
      case 's':
        puts( *(argument++) );
        break;

      /* Print a decimal number. */
      case 'd':
        itoa( (int)(*(argument++)), 10, ITOA_NULL );
        break;
      case 'x':
        itoa( (int)(*(argument++)), 16, ITOA_LOWERCASE );
        break;
      case 'X':
        itoa( (int)(*(argument++)), 16, ITOA_UPPERCASE );
        break;
      case 'o':
        itoa( (int)(*(argument++)), 8, ITOA_NULL );
        break;
      }
      p++;

    /* Print a % if it is escaped. */
    } else if( *p == '\\' && *(p+1) == '%' ) {
        putchar( '%' );
    } else {
      putchar( *(p++) );
    }
  }
}

/* Uses the current console to print a character. */
void putchar( int c )
{
  char str[1];

  str[0] = c;

  if( cur_con != NULL )
    (cur_con->write)( str, 1 );
}
