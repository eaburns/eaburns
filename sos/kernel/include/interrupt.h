#ifndef _TRAP_H_
#define _TRAP_H_

#define ARCH_TRAP(x,y) _ ## x ## _ ## y

#ifdef __ASM__

  .extern interrupt_divide
  .extern interrupt_debug
  .extern interrupt_nmi
  .extern interrupt_break
  .extern interrupt_overflow
  .extern interrupt_bound
  .extern interrupt_illegal
  .extern interrupt_fpu
  .extern interrupt_doublefault
  .extern interrupt_cso
  .extern interrupt_invalidtts
  .extern interrupt_segnotpres
  .extern interrupt_stack
  .extern interrupt_genpro
  .extern interrupt_page
  .extern interrupt_reserved
  .extern interrupt_floatpt
  .extern interrupt_align
  .extern interrupt_machine
  .extern interrupt_syscall

#else

#include <ktypes.h>
#include <regs.h>

/* Enable/Disable interrupts. */
void __inline enable();
void __inline disable();

void interrupt_divide( struct regs r, uint32 ip, uint32 cs, uint32 eflags );
void interrupt_debug( struct regs r, uint32 ip, uint32 cs, uint32 eflags );
void interrupt_nmi( struct regs r, uint32 ip, uint32 cs, uint32 eflags );
void interrupt_break( struct regs r, uint32 ip, uint32 cs, uint32 eflags );
void interrupt_overflow( struct regs r, uint32 ip, uint32 cs, uint32 eflags );
void interrupt_bound( struct regs r, uint32 ip, uint32 cs, uint32 eflags );
void interrupt_illegal( struct regs r, uint32 ip, uint32 cs, uint32 eflags );
void interrupt_fpu( struct regs r, uint32 ip, uint32 cs, uint32 eflags ); 
void interrupt_doublefault( struct regs r, uint32 code, uint32 ip, uint32 cs, uint32 eflags );
void interrupt_cso( struct regs r, uint32 ip, uint32 cs, uint32 eflags ); 
void interrupt_invalidtts( struct regs r, uint32 code, uint32 ip, uint32 cs, uint32 eflags );
void interrupt_segnotpres( struct regs r, uint32 code, uint32 ip, uint32 cs, uint32 eflags );
void interrupt_stack( struct regs r, uint32 code, uint32 ip, uint32 cs, uint32 eflags );
void interrupt_genpro( struct regs r, uint32 code, uint32 ip, uint32 cs, uint32 eflags );
void interrupt_page( struct regs r, uint32 code, uint32 ip, uint32 cs, uint32 eflags );
void interrupt_reserved( struct regs r, uint32 ip, uint32 cs, uint32 eflags );
void interrupt_floatpt( struct regs r, uint32 ip, uint32 cs, uint32 eflags );
void interrupt_align( struct regs r, uint32 ip, uint32 cs, uint32 eflags );    
void interrupt_machine( struct regs r, uint32 ip, uint32 cs, uint32 eflags );

void interrupt_irq( uint32 num, struct regs r, word_t ip );

void interrupt_syscall( struct regs r, uint32 ip, uint32 cs, uint32 eflags );
#endif /* __ASM__ */

#endif /* _TRAP_H_ */
