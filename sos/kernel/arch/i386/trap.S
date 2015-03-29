.text
#define __ASM__
#include <interrupt.h>

#define PUSHA \
  pushl %eax; \
  pushl %ecx; \
  pushl %ebx; \
  pushl %edx; \
  pushl %ebp; \
  pushl %esi; \
  pushl %edi

#define POPA \
  popl %edi; \
  popl %esi; \
  popl %ebp; \
  popl %edx; \
  popl %ebx; \
  popl %ecx; \
  popl %eax

#define TRAP(x) .globl ARCH_TRAP(i386,x);\
                ARCH_TRAP(i386,x):;\
                cli; \
                PUSHA; \
                call x;\
                POPA;\
                iret

#define TRAP_WITH_CODE(x) .globl ARCH_TRAP(i386,x);\
                ARCH_TRAP(i386,x):;\
                cli; \
                PUSHA;\
                call x;\
                POPA;\
                add $4, %esp; \
                iret

#define TRAP_IRQ(x, y) .globl ARCH_TRAP(i386,x);\
                ARCH_TRAP(i386,x):;\
                cli; \
                PUSHA;\
                pushl $y;\
                call __i386_interrupt_irq_short;\
                addl $4, %esp; \
                POPA;\
                iret

TRAP(interrupt_divide)
TRAP(interrupt_debug)
TRAP(interrupt_nmi)
TRAP(interrupt_break)
TRAP(interrupt_overflow)
TRAP(interrupt_bound)
TRAP(interrupt_illegal)
TRAP(interrupt_fpu)
TRAP_WITH_CODE(interrupt_doublefault)
TRAP(interrupt_cso)
TRAP_WITH_CODE(interrupt_invalidtts)
TRAP_WITH_CODE(interrupt_segnotpres)
TRAP_WITH_CODE(interrupt_stack)
TRAP_WITH_CODE(interrupt_genpro)
TRAP_WITH_CODE(interrupt_page)
TRAP(interrupt_reserved)
TRAP(interrupt_floatpt)
TRAP(interrupt_align)
TRAP(interrupt_machine)
TRAP(interrupt_syscall)
TRAP_IRQ(interrupt_irq_0, 0)
TRAP_IRQ(interrupt_irq_1, 1)
TRAP_IRQ(interrupt_irq_2, 2)
TRAP_IRQ(interrupt_irq_3, 3)
TRAP_IRQ(interrupt_irq_4, 4)
TRAP_IRQ(interrupt_irq_5, 5)
TRAP_IRQ(interrupt_irq_6, 6)
TRAP_IRQ(interrupt_irq_7, 7)
TRAP_IRQ(interrupt_irq_8, 8)
TRAP_IRQ(interrupt_irq_9, 9)
TRAP_IRQ(interrupt_irq_10, 10)
TRAP_IRQ(interrupt_irq_11, 11)
TRAP_IRQ(interrupt_irq_12, 12)
TRAP_IRQ(interrupt_irq_13, 13)
TRAP_IRQ(interrupt_irq_14, 14)
TRAP_IRQ(interrupt_irq_15, 15)
