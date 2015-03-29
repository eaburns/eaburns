#include <sched.h>
#include <process.h>
#include <i386proc.h>
#include <descriptor.h>

#include <kstdio.h>

extern global_descriptor gdt[];

void context_switch( process_t *to, process_t *from )
{
  uint32 selector[2];
  selector[1] = (TSS_SELECTOR+(to->pid*8))|3;

  /* Make this TSS un-bussy. */
  gdt[(TSS_SELECTOR/8)+to->pid].type = 0x9;

  __asm__ __volatile__( "ljmp *%0" : : "m"(*selector) );
}

void arch_sched_add( process_t *p )
{
  __set_selector( &gdt[(TSS_SELECTOR/8)+p->pid],
                  (uint32)((proc_info_t*)p->arch_info)->tss, 
                  TSS_SIZE, 0x9, DPL_USER );
}
