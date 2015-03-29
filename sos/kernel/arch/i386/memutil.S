.extern kprintf

.text
.globl memcpy
memcpy:
  pushl %ebp
  movl %esp, %ebp

  movl 8(%ebp), %edi
  movl 12(%ebp), %esi
  movl 16(%ebp), %ecx

  cld
  rep
  movsb

  popl %ebp
  ret

.globl memset
memset:
  pushl %ebp
  movl %esp, %ebp

  movl 8(%ebp), %edi

  movl 12(%ebp), %eax
  movl 16(%ebp), %ecx

  cld
  rep
  stosb 

  popl %ebp
  ret
