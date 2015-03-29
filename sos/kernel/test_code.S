  .align 4
  .globl test_thread_1
test_thread_1:
  movl $100, %ecx
1:
  movl $2, %eax
  movl $1, %ebx
  int $0x80

/* --- wait --- */
  pushl %ecx
  movl $0x9999, %ecx
2:
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  decl %ecx
  jnz 2b

  popl %ecx
/* --- wait end --- */

  decl %ecx
  jnz 1b

  ret

  .globl test_thread_1_end
test_thread_1_end:

/******************************************************************************/

  .globl test_thread_2
test_thread_2:
  movl $200, %ecx
1:
  movl $2, %eax
  movl $2, %ebx
  int $0x80

/* --- wait --- */
  pushl %ecx
  movl $9999, %ecx
2:
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  decl %ecx
  jnz 2b

  popl %ecx
/* --- wait end --- */

  decl %ecx
  jnz 1b

  ret
  .globl test_thread_2_end
test_thread_2_end:

/******************************************************************************/

  .globl test_thread_3
test_thread_3:
  movl $20, %ecx
1:
  movl $2, %eax
  movl $3, %ebx
  int $0x80

/* --- wait --- */
  pushl %ecx
  movl $9999, %ecx
2:
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  decl %ecx
  jnz 2b

  popl %ecx
/* --- wait end --- */

  decl %ecx
  jnz 1b

  ret
  .globl test_thread_3_end
test_thread_3_end:
