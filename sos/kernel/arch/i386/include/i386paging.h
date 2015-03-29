#ifndef _I386PAGING_H_
#define _I386PAGING_H_

#define PG_PRESENT 1
#define PG_READWRITE 1<<1
#define PG_USER 1<<2

/* CR0 paging flag */
#define CR0_PG_FLAG 1<<31

/* Base address of the page directory. */
#ifndef __ASM__
  #define PD_BASE ((word_t**)&__page_dir)
#else
  #define PD_BASE __page_dir
#endif /* __ASM__ */

#ifndef __ASM__

#include <ktypes.h>

/* Initialize kernel page table, and make cr3 point to it. */
int init_paging();

/* Set bit 31 of cr0 (enable paging). */
void enable_paging();

extern uint32 __text_start, __text_end, __data_end, __data_start, __page_dir;

#endif /* __ASM__ */

#endif
