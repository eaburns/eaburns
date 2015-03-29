#include <interrupt.h>
#include <irq.h>
#include <io.h>
#include <kstdio.h>
#include <8259_pic.h>

/* Small wrapper for irqs, resets the 8259 after the routine runs. */
void __i386_interrupt_irq_short( uint32 num, struct regs r, word_t ip )
{
  if( num > 7 ) {
    outb( PIC_1_CMD_PORT, 0x20 );
    outb( PIC_0_CMD_PORT, 0x20 );
  } else {
    outb( PIC_0_CMD_PORT, 0x20 );
  }

  interrupt_irq( num, r, ip ); 
}

/* This is less safe then the previous function, it gives the pic the okay
 * on another irq _BEFORE_ running this irq routing. */
void __i386_interrupt_irq_long( uint32 num, struct regs r, word_t ip )
{
  if( num > 7 ) {
    outb( PIC_1_CMD_PORT, 0x20 );
    outb( PIC_0_CMD_PORT, 0x20 );
  } else {
    outb( PIC_0_CMD_PORT, 0x20 );
  }

  interrupt_irq( num, r, ip ); 
}
