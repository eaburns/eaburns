#include "interrupt.h"

/* Declarations for all architecture trap wrapper functions. */
void ARCH_TRAP(i386,interrupt_divide)();
void ARCH_TRAP(i386,interrupt_debug)();
void ARCH_TRAP(i386,interrupt_nmi)();
void ARCH_TRAP(i386,interrupt_break)();
void ARCH_TRAP(i386,interrupt_overflow)();
void ARCH_TRAP(i386,interrupt_bound)();
void ARCH_TRAP(i386,interrupt_illegal)();
void ARCH_TRAP(i386,interrupt_fpu)();
void ARCH_TRAP(i386,interrupt_doublefault)();
void ARCH_TRAP(i386,interrupt_cso)();
void ARCH_TRAP(i386,interrupt_invalidtts)();
void ARCH_TRAP(i386,interrupt_segnotpres)();
void ARCH_TRAP(i386,interrupt_stack)();
void ARCH_TRAP(i386,interrupt_genpro)();
void ARCH_TRAP(i386,interrupt_page)();
void ARCH_TRAP(i386,interrupt_reserved)();
void ARCH_TRAP(i386,interrupt_floatpt)();
void ARCH_TRAP(i386,interrupt_align)();
void ARCH_TRAP(i386,interrupt_machine)();

void ARCH_TRAP(i386,interrupt_irq_0)();
void ARCH_TRAP(i386,interrupt_irq_1)();
void ARCH_TRAP(i386,interrupt_irq_2)();
void ARCH_TRAP(i386,interrupt_irq_3)();
void ARCH_TRAP(i386,interrupt_irq_4)();
void ARCH_TRAP(i386,interrupt_irq_5)();
void ARCH_TRAP(i386,interrupt_irq_6)();
void ARCH_TRAP(i386,interrupt_irq_7)();
void ARCH_TRAP(i386,interrupt_irq_8)();
void ARCH_TRAP(i386,interrupt_irq_9)();
void ARCH_TRAP(i386,interrupt_irq_10)();
void ARCH_TRAP(i386,interrupt_irq_11)();
void ARCH_TRAP(i386,interrupt_irq_12)();
void ARCH_TRAP(i386,interrupt_irq_13)();
void ARCH_TRAP(i386,interrupt_irq_14)();
void ARCH_TRAP(i386,interrupt_irq_15)();

void ARCH_TRAP(i386,interrupt_syscall)();
