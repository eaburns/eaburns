#include <ktypes.h>

/* Gets input from port number ``port'' and returns the value. */
uint8 inb( unsigned short port );

/* Outputs ``val'' to port ``port.'' */
void outb( unsigned short port, uint8 val );
