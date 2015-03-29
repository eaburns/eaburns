#ifndef _IRQ_H_
#define _IRQ_H_

#include <ktypes.h>
#include <regs.h>

/* Greatest numbered irq. */
#define MAX_IRQ_NUM 15

/* Interrupt call back function type. */
typedef int (*intcallback_t)( struct regs, word_t ip );

/* Binds an irq to a function. */
uint32 bind_irq( uint16 num, void *handler );

/* Enables/Disables an irq. */
int enable_irq( uint16 num );
int disable_irq( uint16 num );


#endif /* _IRQ_H_ */
