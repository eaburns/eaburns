#define __ASM__

#include <paging.h>
#include <i386paging.h>

.globl enable_paging
enable_paging:


  movl %cr0, %eax
  orl $CR0_PG_FLAG, %eax
  movl %eax, %cr0

  jmp near_jump

near_jump:

  ret
