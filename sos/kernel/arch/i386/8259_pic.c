#include <8259_pic.h>
#include <irq.h>
#include <io.h>
#include <kstdio.h>
#include <descriptor.h>
#include <retval.h>

int init_pic()
{
  /* ICW1 */
  outb( 0x20, 0x11 ); /* Edge triggered irqs. */
  outb( 0xA0, 0x11 ); /* Cascade mode (use ICW4). */

  /* ICW2 */
  outb( 0x21, 0x28 ); /* irq 0-7 begins at interrupt 0x28. */
  outb( 0xA1, 0x30 ); /* irq 8-15 begins at interrupt 0x30. */

  /* ICW3 */
  outb( 0x21, 0x04 ); /* Tell master slave is on IRQ 2. */
  outb( 0xA1, 0x02 ); /* Tell slave it is on master's IRQ 2. */

  /* ICW4 */
  outb( 0x21, 0x01 );
  outb( 0xA1, 0x01 );
 
  /* Disable these interrupts for now (except the cascade). */
  outb( PIC_0_PORT, ~(1<<2) );
  outb( PIC_1_PORT, ~0 );

  return RET_SUCCESS;
}

int enable_irq( uint16 num )
{
  if( num < PIC_0_MAX_IRQ )
    outb( PIC_0_PORT, inb( PIC_0_PORT ) & ~(1<<num) );
  else 
    outb( PIC_1_PORT, inb( PIC_1_PORT ) & ~(1<<(num-PIC_0_MAX_IRQ+1)) );

  return RET_SUCCESS;
}

int disable_irq( uint16 num )
{
  if( num < PIC_0_MAX_IRQ )
    outb( PIC_0_PORT, inb( PIC_0_PORT ) | ~(1<<num) );
  else 
    outb( PIC_1_PORT, inb( PIC_1_PORT ) | ~(1<<(num-PIC_0_MAX_IRQ+1)) );

  return RET_SUCCESS;;
}
