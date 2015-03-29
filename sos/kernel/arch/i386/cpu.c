#include <cpu.h>
#include <irq.h>
#include <trap.h>
#include <interrupt.h>
#include <descriptor.h>
#include <memutil.h>
#include <i386cpu.h>
#include <8259_pic.h>
#include <8253_timer.h>
#include <timer.h>
#include <kstdio.h>
#include <i386paging.h>
#include <paging.h>
#include <kalloc.h>
#include <retval.h>
#include <init.h>
#include <panic.h>
#include <tss.h>
#include <memman.h>
#include <sched.h>  /* SCHED_MAX_PROCS */

/* Number of entries in the IDT. */
#define IDT_ENTRIES 129

interrupt_descriptor idt[IDT_ENTRIES];
table_descriptor idtd;

/* Number of entries in the GDT, null descriptor, 2 kernel descriptors, 
 * 2 user descriptors, and 1 TSS descriptor for each possible. */
#define GDT_ENTRIES (6+SCHED_MAX_PROCS)

global_descriptor gdt[GDT_ENTRIES];
table_descriptor gdtd;
tss_t system_tss;

/* Defined in linker script. */
extern void* __kalloc_base;

/* Setup the system_tss. */
static void init_tss()
{
  bzero( &system_tss, TSS_SIZE );
  system_tss.eflags = 0x0;
  system_tss.bitmap = TSS_SIZE; 
  system_tss.esp = 0x9FBFF;
  system_tss.esp0 = 0x9FBFF;
  system_tss.esp1 = 0x9FBFF;
  system_tss.esp2 = 0x9FBFF;
  system_tss.eip = (uint32)ARCH_TRAP(i386,interrupt_irq_0);
  system_tss.ss = KDATA_SELECTOR;
  system_tss.ss0 = KDATA_SELECTOR;
  system_tss.ss1 = KDATA_SELECTOR;
  system_tss.ss2 = KDATA_SELECTOR;
  system_tss.ds = KDATA_SELECTOR;
  system_tss.es = KDATA_SELECTOR;
  system_tss.fs = KDATA_SELECTOR;
  system_tss.gs = KDATA_SELECTOR;
  system_tss.cs = KCODE_SELECTOR;
}

void __inline halt();

int init_cpu()
{
  int i;

  /* Initialize GDT. */
  bzero( gdt, sizeof(global_descriptor)*GDT_ENTRIES );

  /* Kernel code and data (ring 0). */
  __set_selector( &gdt[KCODE_SELECTOR/8], 0, 0xfffff, GDT_ER, DPL_KERNEL );
  __set_selector( &gdt[KDATA_SELECTOR/8], 0, 0xfffff, GDT_RW, DPL_KERNEL );

  /* User code and data (ring 3). */
  __set_selector( &gdt[UCODE_SELECTOR/8], 0, 0xfffff, GDT_ER, DPL_USER );
  __set_selector( &gdt[UDATA_SELECTOR/8], 0, 0xfffff, GDT_RW, DPL_USER );

  /* TSS descriptor. */
  init_tss();
  __set_selector( &gdt[TSS_SELECTOR/8], (uint32)&system_tss, 
                  TSS_SIZE, 0x9, DPL_KERNEL );

  gdtd.base = (uint32)gdt;
  gdtd.limit = sizeof(global_descriptor)*GDT_ENTRIES;
  lgdt( &gdtd );

  /* Load TSS descriptor. */
  i = TSS_SELECTOR;
  __asm__ __volatile__( "ltr %%ax" : : "a"(i) );

  /* Initialize the interrupt descriptor table and load it. */
  bzero( idt, sizeof(interrupt_descriptor)*IDT_ENTRIES );
  set_interrupt( &idt[0], ARCH_TRAP(i386,interrupt_divide), KCODE_SELECTOR, 
                 DESC_TYPE_TRAP_GATE, DPL_KERNEL );
  set_interrupt( &idt[1], ARCH_TRAP(i386,interrupt_debug), KCODE_SELECTOR, 
                 DESC_TYPE_TRAP_GATE, DPL_KERNEL );
  set_interrupt( &idt[2], ARCH_TRAP(i386,interrupt_nmi), KCODE_SELECTOR, 
                 DESC_TYPE_TRAP_GATE, DPL_KERNEL );
  set_interrupt( &idt[3], ARCH_TRAP(i386,interrupt_break), KCODE_SELECTOR, 
                 DESC_TYPE_TRAP_GATE, DPL_KERNEL );
  set_interrupt( &idt[4], ARCH_TRAP(i386,interrupt_overflow), KCODE_SELECTOR, 
                 DESC_TYPE_TRAP_GATE, DPL_KERNEL );
  set_interrupt( &idt[5], ARCH_TRAP(i386,interrupt_bound), KCODE_SELECTOR, 
                 DESC_TYPE_TRAP_GATE, DPL_KERNEL );
  set_interrupt( &idt[6], ARCH_TRAP(i386,interrupt_illegal), KCODE_SELECTOR, 
                 DESC_TYPE_TRAP_GATE, DPL_KERNEL );
  set_interrupt( &idt[7], ARCH_TRAP(i386,interrupt_fpu), KCODE_SELECTOR, 
                 DESC_TYPE_TRAP_GATE, DPL_KERNEL );
  set_interrupt( &idt[8], ARCH_TRAP(i386,interrupt_doublefault), KCODE_SELECTOR, 
                 DESC_TYPE_TRAP_GATE, DPL_KERNEL );
  set_interrupt( &idt[9], ARCH_TRAP(i386,interrupt_cso), KCODE_SELECTOR, 
                 DESC_TYPE_TRAP_GATE, DPL_KERNEL );
  set_interrupt( &idt[10], ARCH_TRAP(i386,interrupt_invalidtts), KCODE_SELECTOR, 
                 DESC_TYPE_TRAP_GATE, DPL_KERNEL );
  set_interrupt( &idt[11], ARCH_TRAP(i386,interrupt_segnotpres), KCODE_SELECTOR, 
                 DESC_TYPE_TRAP_GATE, DPL_KERNEL );
  set_interrupt( &idt[12], ARCH_TRAP(i386,interrupt_stack), KCODE_SELECTOR, 
                 DESC_TYPE_TRAP_GATE, DPL_KERNEL );
  set_interrupt( &idt[13], ARCH_TRAP(i386,interrupt_genpro), KCODE_SELECTOR, 
                 DESC_TYPE_TRAP_GATE, DPL_KERNEL );
  set_interrupt( &idt[14], ARCH_TRAP(i386,interrupt_page), KCODE_SELECTOR, 
                 DESC_TYPE_TRAP_GATE, DPL_KERNEL );
  set_interrupt( &idt[15], ARCH_TRAP(i386,interrupt_reserved), KCODE_SELECTOR, 
                 DESC_TYPE_TRAP_GATE, DPL_KERNEL );
  set_interrupt( &idt[16], ARCH_TRAP(i386,interrupt_floatpt), KCODE_SELECTOR, 
                 DESC_TYPE_TRAP_GATE, DPL_KERNEL );
  set_interrupt( &idt[17], ARCH_TRAP(i386,interrupt_align), KCODE_SELECTOR, 
                 DESC_TYPE_TRAP_GATE, DPL_KERNEL );
  set_interrupt( &idt[18], ARCH_TRAP(i386,interrupt_machine), KCODE_SELECTOR, 
                 DESC_TYPE_TRAP_GATE, DPL_KERNEL );

  for( i = 19; i < 32; i++ )
    set_interrupt( &idt[i], ARCH_TRAP(i386,interrupt_reserved), KCODE_SELECTOR, 
                   DESC_TYPE_TRAP_GATE, DPL_KERNEL );

  for( i = 32; i < 40; i++ )
    set_interrupt( &idt[i], ARCH_TRAP(i386,interrupt_reserved), KCODE_SELECTOR, 
                   DESC_TYPE_TRAP_GATE, DPL_KERNEL );

  /* Set the irq callback. */
  set_interrupt( &idt[40], ARCH_TRAP(i386,interrupt_irq_0), KCODE_SELECTOR, 
                 DESC_TYPE_TRAP_GATE, DPL_KERNEL );
  set_interrupt( &idt[41], ARCH_TRAP(i386,interrupt_irq_1), KCODE_SELECTOR, 
                 DESC_TYPE_TRAP_GATE, DPL_KERNEL );
  set_interrupt( &idt[42], ARCH_TRAP(i386,interrupt_irq_2), KCODE_SELECTOR, 
                 DESC_TYPE_TRAP_GATE, DPL_KERNEL );
  set_interrupt( &idt[43], ARCH_TRAP(i386,interrupt_irq_3), KCODE_SELECTOR, 
                 DESC_TYPE_TRAP_GATE, DPL_KERNEL );
  set_interrupt( &idt[44], ARCH_TRAP(i386,interrupt_irq_4), KCODE_SELECTOR, 
                 DESC_TYPE_TRAP_GATE, DPL_KERNEL );
  set_interrupt( &idt[45], ARCH_TRAP(i386,interrupt_irq_5), KCODE_SELECTOR, 
                 DESC_TYPE_TRAP_GATE, DPL_KERNEL );
  set_interrupt( &idt[46], ARCH_TRAP(i386,interrupt_irq_6), KCODE_SELECTOR, 
                 DESC_TYPE_TRAP_GATE, DPL_KERNEL );
  set_interrupt( &idt[47], ARCH_TRAP(i386,interrupt_irq_7), KCODE_SELECTOR, 
                 DESC_TYPE_TRAP_GATE, DPL_KERNEL );
  set_interrupt( &idt[48], ARCH_TRAP(i386,interrupt_irq_8), KCODE_SELECTOR, 
                 DESC_TYPE_TRAP_GATE, DPL_KERNEL );
  set_interrupt( &idt[48], ARCH_TRAP(i386,interrupt_irq_9), KCODE_SELECTOR, 
                 DESC_TYPE_TRAP_GATE, DPL_KERNEL );
  set_interrupt( &idt[50], ARCH_TRAP(i386,interrupt_irq_10), KCODE_SELECTOR, 
                 DESC_TYPE_TRAP_GATE, DPL_KERNEL );
  set_interrupt( &idt[51], ARCH_TRAP(i386,interrupt_irq_11), KCODE_SELECTOR, 
                 DESC_TYPE_TRAP_GATE, DPL_KERNEL );
  set_interrupt( &idt[52], ARCH_TRAP(i386,interrupt_irq_12), KCODE_SELECTOR, 
                 DESC_TYPE_TRAP_GATE, DPL_KERNEL );
  set_interrupt( &idt[53], ARCH_TRAP(i386,interrupt_irq_13), KCODE_SELECTOR, 
                 DESC_TYPE_TRAP_GATE, DPL_KERNEL );
  set_interrupt( &idt[54], ARCH_TRAP(i386,interrupt_irq_14), KCODE_SELECTOR, 
                 DESC_TYPE_TRAP_GATE, DPL_KERNEL );
  set_interrupt( &idt[55], ARCH_TRAP(i386,interrupt_irq_15), KCODE_SELECTOR, 
                 DESC_TYPE_TRAP_GATE, DPL_KERNEL );

  for( i = 40+MAX_IRQ_NUM+1; i < 128; i++ )
    set_interrupt( &idt[i], ARCH_TRAP(i386,interrupt_reserved), KCODE_SELECTOR, 
                   DESC_TYPE_TRAP_GATE, DPL_KERNEL );

  set_interrupt( &idt[128], ARCH_TRAP(i386,interrupt_syscall), KCODE_SELECTOR, 
                 DESC_TYPE_INTR_GATE, DPL_USER );

  /* Load the table. */
  idtd.base = (uint32)idt;
  idtd.limit = sizeof(interrupt_descriptor)*IDT_ENTRIES;
  lidt( &idtd );

  INITIALIZE( init_pic, "Unable to initialize i8259 PIC." );
  INITIALIZE( init_timer, "Unable to initialize i8253 Timer." );
  timer_enable(); /* YET interrupts are still masked. */

  /* 64k for kalloc. */
  if( kalloc_init( &__kalloc_base, (uint32)(&__kalloc_base) + 0x10000 ) == 
      RET_FAILURE ) { panic( "Unable to initialize kalloc." ); }

  INITIALIZE( init_memman, "Unable to initialize memory management." );

  /* Paging requires memman to be inited. */
  INITIALIZE( init_paging, "Unable to initialize paging." );

  return RET_SUCCESS;
}

void __inline enable()
{
  __asm__ __volatile( "sti" );
}

void __inline disable()
{
  __asm__ __volatile( "cli" );
}

void __inline halt()
{
  __asm__ __volatile( "hlt" );
}

void __inline lidt( table_descriptor *idtd )
{
  __asm__ __volatile( "lidt (%0)" : : "r" (idtd) );
}

void __inline lgdt( table_descriptor *idtd )
{
  /* Load new GDT. */
  __asm__ __volatile( "lgdt (%0)" : : "r" (idtd) );
}
