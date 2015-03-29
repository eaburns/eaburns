#include <io.h>

uint8 inb( unsigned short port )
{
  uint8 retval;

  __asm__( "in %%dx, %%al" : "=a"(retval) : "d"(port) );

  return retval;
}

void outb( unsigned short port, uint8 val )
{
  __asm__( "out %%al, %%dx" : : "a"(val), "d"(port) );
}
