#ifndef _PANIC_H_
#define _PANIC_H_

#include <interrupt.h> /* struct regs */

/* Halts the kernel with a message, and possibly dumps regs. */
void panic( char *msg );
             

#endif /* _PANIC_H_ */
