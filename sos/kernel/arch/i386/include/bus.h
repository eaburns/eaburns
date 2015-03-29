#ifndef _BUS_H_
#define _BUS_H_

#include <ktypes.h>
#include <io.h>

#define I386_BUS_IO_T 1  /* bus space is an I/O port */
#define I386_BUS_MEM_T 2 /* bus space is memory. */

/* These functions read/write to/from bus space.  This idea is from BSD. */

/* Reads an 8 bit byte from the bus space. */
#define bus_space_read_8( type, base, off, buff ) \
do {\
  if( type == I386_BUS_IO_T ) { \
    buff = inb( (uint32)base+off ); \
  } else { \
    buff = (*((uint8*)(base+off))); \
  } \
}while( 0 ) 


/* Writes an 8 bit quantity to the bus space. */
#define bus_space_write_8( type, base, off, val ) \
do {\
  if( type == I386_BUS_IO_T ) { \
    outb( (uint32)base+off, val ); \
  } else { \
    (*((uint8*)(base+off))) = val; \
  } \
}while( 0 )
  

#endif /* _BUS_H_ */
