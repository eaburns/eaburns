  .data

.globl mem_size
mem_size:
  .word 0

  .text
  .globl _start
_start:

  /* call the kernel. */
  call kmain

/* Attempt to halt the cpu. */
halt:
  hlt
  jmp halt
