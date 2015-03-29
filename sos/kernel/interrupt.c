#include <interrupt.h>
#include <irq.h>
#include <ktypes.h>
#include <panic.h>
#include <kstdio.h>
#include <kstdlib.h>
#include <config/kern.h>
#include <retval.h>
#include <cpu.h>
#include <sched.h>

void interrupt_divide( struct regs r, uint32 ip, uint32 cs, uint32 eflags )
{
  panic( "Divide by zero exception.\n" );
}

void interrupt_debug( struct regs r, uint32 ip, uint32 cs, uint32 eflags )
{
  panic( "Debug exception.\n" );
}

void interrupt_nmi( struct regs r, uint32 ip, uint32 cs, uint32 eflags )
{
  panic( "NMI exception.\n" );
}

void interrupt_break( struct regs r, uint32 ip, uint32 cs, uint32 eflags )
{
  panic( "Break exception.\n" );
}

void interrupt_overflow( struct regs r, uint32 ip, uint32 cs, uint32 eflags )
{
  panic( "Overflow exception.\n" );
}

void interrupt_bound( struct regs r, uint32 ip, uint32 cs, uint32 eflags )
{
  panic( "Bound exception.\n" );
}

void interrupt_illegal( struct regs r, uint32 ip, uint32 cs, uint32 eflags )
{
  kprintf( "Illegal instruction exception at ip: %d (0x%x).\n", ip, ip );
  panic("");
}

void interrupt_fpu( struct regs r, uint32 ip, uint32 cs, uint32 eflags )
{
  panic( "FPU exctition.\n" );
}

void interrupt_doublefault( struct regs r, uint32 code, uint32 ip, uint32 cs, 
              uint32 eflags )
{
  panic( "Double fault exception.\n" );
}

void interrupt_cso( struct regs r, uint32 ip, uint32 cs, uint32 eflags )
{
  panic( "Coprocessor segment overrun exception.\n" );
}

void interrupt_invalidtts( struct regs r, uint32 code, uint32 ip, uint32 cs, 
                           uint32 eflags )
{
  panic( "Invalid TTS exception.\n" );
}

void interrupt_segnotpres( struct regs r, uint32 code, uint32 ip, uint32 cs, 
                           uint32 eflags )
{
  panic( "Segment not present exception.\n" );
}

void interrupt_stack( struct regs r, uint32 code, uint32 ip, uint32 cs, 
                      uint32 eflags )
{
  panic( "Stack exception.\n" );
}

void interrupt_genpro( struct regs r, uint32 code, uint32 ip, uint32 cs, 
                       uint32 eflags )
{
  panic( "General protection fault exception.\n" );
}

#define PF_ERROR_P  1 /* Page not present flag. */
#define PF_ERROR_RW 2 /* Read/write permission error flag. */
#define PF_ERROR_US 4 /* User/Supervisor permission error flag. */
#define PF_ERROR_RSVD 8 /* Reservation bit violation. */

void interrupt_page( struct regs r, uint32 code, uint32 ip, uint32 cs, 
                     uint32 eflags )
{
#ifdef ARCH_I386
  uint32 cr2;
#endif /* ARCH_I386 */

/*
 * Parsing this error code is probably very intel specific.
 */
#ifdef ARCH_I386
#ifdef I386_VERBOSE_PAGE_FAULT
  __asm__ __volatile__( "movl %%cr2, %%eax; movl %%eax, %0" : "=m"(cr2) : );
  kprintf( "->Page fault for virtual address: 0x%x (%d).\n", cr2, cr2 );

  if( code&PF_ERROR_P ) {
    kprintf("->Page access violation.\n");
  } else {
    kprintf("->Page not present.\n");
  }

  if( code&PF_ERROR_US ) {
    kprintf("->Exception caused in user mode.\n");
  } else {
    kprintf("->Exception caused in supervisor mode.\n");
  }

  if( code&PF_ERROR_RSVD ) {
    kprintf("->Page reservation bit violation.\n");
  }

  if( code&PF_ERROR_RW ) {
    kprintf("->Page violation caused by a write operation.\n" ); 
  } else {
    kprintf("->Page violation caused by a read operation.\n" ); 
  }
#endif /* I386_VERBOSE_PAGE_FAULT */
#endif /* ARCH_I386 */

  kprintf( "Halting.\n" );
  disable(); halt();
}

void interrupt_reserved( struct regs r, uint32 ip, uint32 cs, uint32 eflags )
{
  panic( "Reserved exception.\n" );
}

void interrupt_floatpt( struct regs r, uint32 ip, uint32 cs, uint32 eflags )
{
  panic( "Floating point exception.\n" );
}

void interrupt_align( struct regs r, uint32 ip, uint32 cs, uint32 eflags )
{
  panic( "Alignment check exception.\n" );
}

void interrupt_machine( struct regs r, uint32 ip, uint32 cs, uint32 eflags )
{
  panic( "Machine check exception.\n" );
}

void interrupt_syscall( struct regs r, uint32 ip, uint32 cs, uint32 eflags )
{
  switch( r.eax ) {
  case 1:
    process_kill( curproc );

    /* This shouldn't be necessairy. */
    kprintf( "process_kill returned...\n" );
    break;

  case 2: 
    kprintf( "%x", r.ebx );
  }
  
}

void interrupt_irq( uint32 num, struct regs r, word_t ip )
{
  extern intcallback_t irqs[MAX_IRQ_NUM];
  intcallback_t p; 

  if( irqs[num] != NULL ) {
    p = irqs[num];
    (*p)( r, ip );
  }
}
