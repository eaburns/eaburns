
/* Run Time System Function Declarations */
	.extern RTS_outputInteger
	.extern RTS_checkForZeroDivide
	.extern RTS_reverseArgumentsOnStack
	.extern RTS_checkMalloc
	.extern RTS_checkNullReference
	.extern RTS_checkNarrowingConversion
	.extern RTS_createArray

/* VMTs */
	.data
	.globl RTS_arrayVMT
RTS_arrayVMT:
	.long 0
	.long Object$$destructor
	.long Object$equals$Object$
A$VMT:
	.long Object$VMT
	.long A$$destructor
	.long Object$equals$Object$

B$VMT:
	.long A$VMT
	.long B$$destructor
	.long Object$equals$Object$

Object$VMT:
	.long 0
	.long Object$$destructor
	.long Object$equals$Object$


	.text

/* Main Function Begin */
	.align 4
	.globl main
main:
	pushl %ebp
	movl %esp, %ebp

/* Variable Declarators */
	.data
mainvar$b: .long 0
	.text

/* Main Variable Useage */
	pushl $mainvar$b

/* Class Creation */
	pushl $0
	call RTS_reverseArgumentsOnStack
	addl $4, %esp
	pushl $1
	pushl $16
	call calloc
	addl $8, %esp
	pushl %eax
	pushl $22
	call RTS_checkMalloc
	addl $4, %esp
	popl %eax
	movl $B$VMT, (%eax)
	pushl %eax
	call B$$B$
	popl %eax
	addl $0, %esp
	pushl %eax

/* Binary Operation: Assignment */
	popl %eax
	popl %edx
	movl %eax, (%edx)
	pushl %eax

/* Expression Statement Cleanup */
	addl $4, %esp

/* Main Variable Useage */
	pushl $mainvar$b

/* Deref */
	popl %eax
	movl (%eax), %eax
	pushl %eax

/* Delete */
	popl %eax
	pushl %eax
	pushl $23
	call RTS_checkNullReference
	addl $4, %esp
	popl %eax
	pushl %eax
	movl (%eax), %eax
	addl $4, %eax
	movl (%eax), %eax
	call *%eax
	call free
	addl $4, %esp

/* Variable Declarators */
	.data
mainvar$a: .long 0
	.text

/* Main Variable Useage */
	pushl $mainvar$a

/* Class Creation */
	pushl $0
	call RTS_reverseArgumentsOnStack
	addl $4, %esp
	pushl $1
	pushl $16
	call calloc
	addl $8, %esp
	pushl %eax
	pushl $26
	call RTS_checkMalloc
	addl $4, %esp
	popl %eax
	movl $A$VMT, (%eax)
	pushl %eax
	call A$$A$
	popl %eax
	addl $0, %esp
	pushl %eax

/* Binary Operation: Assignment */
	popl %eax
	popl %edx
	movl %eax, (%edx)
	pushl %eax

/* Expression Statement Cleanup */
	addl $4, %esp

/* Main Variable Useage */
	pushl $mainvar$a

/* Deref */
	popl %eax
	movl (%eax), %eax
	pushl %eax

/* Field Access */
	pushl $27
	call RTS_checkNullReference
	addl $4, %esp
	popl %eax
	addl $4, %eax
	pushl %eax

/* Integer Literal */
	pushl $11

/* Binary Operation: Assignment */
	popl %eax
	popl %edx
	movl %eax, (%edx)
	pushl %eax

/* Expression Statement Cleanup */
	addl $4, %esp

/* Main Variable Useage */
	pushl $mainvar$a

/* Deref */
	popl %eax
	movl (%eax), %eax
	pushl %eax

/* Field Access */
	pushl $28
	call RTS_checkNullReference
	addl $4, %esp
	popl %eax
	addl $8, %eax
	pushl %eax

/* Integer Literal */
	pushl $11

/* Binary Operation: Assignment */
	popl %eax
	popl %edx
	movl %eax, (%edx)
	pushl %eax

/* Expression Statement Cleanup */
	addl $4, %esp

/* Main Variable Useage */
	pushl $mainvar$a

/* Deref */
	popl %eax
	movl (%eax), %eax
	pushl %eax

/* Field Access */
	pushl $29
	call RTS_checkNullReference
	addl $4, %esp
	popl %eax
	addl $12, %eax
	pushl %eax

/* Integer Literal */
	pushl $11

/* Binary Operation: Assignment */
	popl %eax
	popl %edx
	movl %eax, (%edx)
	pushl %eax

/* Expression Statement Cleanup */
	addl $4, %esp

/* Main Variable Useage */
	pushl $mainvar$a

/* Deref */
	popl %eax
	movl (%eax), %eax
	pushl %eax

/* Delete */
	popl %eax
	pushl %eax
	pushl $30
	call RTS_checkNullReference
	addl $4, %esp
	popl %eax
	pushl %eax
	movl (%eax), %eax
	addl $4, %eax
	movl (%eax), %eax
	call *%eax
	call free
	addl $4, %esp

/* Integer Literal */
	pushl $0

/* Return */
	popl %eax
	jmp main$exit

/* Main Function End */
main$exit:
	popl %ebp
	ret

/* Destructor */
	.align 4
	.globl A$$destructor
A$$destructor:
	pushl %ebp
	movl %esp, %ebp

/* Field */
	movl 8(%ebp), %eax
	addl $4, %eax
	pushl %eax

/* Deref */
	popl %eax
	movl (%eax), %eax
	pushl %eax

/* Field */
	movl 8(%ebp), %eax
	addl $8, %eax
	pushl %eax

/* Deref */
	popl %eax
	movl (%eax), %eax
	pushl %eax

/* Binary Operation: Addition */
	popl %edx
	popl %eax
	addl %edx, %eax
	pushl %eax

/* Field */
	movl 8(%ebp), %eax
	addl $12, %eax
	pushl %eax

/* Deref */
	popl %eax
	movl (%eax), %eax
	pushl %eax

/* Binary Operation: Addition */
	popl %edx
	popl %eax
	addl %edx, %eax
	pushl %eax

/* Out */
	call RTS_outputInteger
	addl $4, %esp

/* Return */
	jmp A$$destructor$exit
A$$destructor$exit:
	leal Object$VMT, %eax
	addl $4, %eax
	movl (%eax), %eax
	call *%eax
	popl %ebp
	ret

/* Constructor Declaration */
	.align 4
	.globl A$$A$
A$$A$:
	pushl %ebp
	movl %esp, %ebp

/* Constructor Invocation */
	pushl $0
	call RTS_reverseArgumentsOnStack
	addl $4, %esp
	pushl 8(%ebp)
	call Object$$Object$
	addl $4, %esp

/* Return */
	jmp A$$A$$exit
A$$A$$exit:
	popl %ebp
	ret

/* Destructor */
	.align 4
	.globl B$$destructor
B$$destructor:
	pushl %ebp
	movl %esp, %ebp

/* Integer Literal */
	pushl $33

/* Out */
	call RTS_outputInteger
	addl $4, %esp

/* Return */
	jmp B$$destructor$exit
B$$destructor$exit:
	leal A$VMT, %eax
	addl $4, %eax
	movl (%eax), %eax
	call *%eax
	popl %ebp
	ret

/* Constructor Declaration */
	.align 4
	.globl B$$B$
B$$B$:
	pushl %ebp
	movl %esp, %ebp

/* Constructor Invocation */
	pushl $0
	call RTS_reverseArgumentsOnStack
	addl $4, %esp
	pushl 8(%ebp)
	call A$$A$
	addl $4, %esp

/* Return */
	jmp B$$B$$exit
B$$B$$exit:
	popl %ebp
	ret

/* Destructor */
	.align 4
	.globl Object$$destructor
Object$$destructor:
	pushl %ebp
	movl %esp, %ebp

/* Return */
	jmp Object$$destructor$exit
Object$$destructor$exit:
	popl %ebp
	ret

/* Constructor Declaration */
	.align 4
	.globl Object$$Object$
Object$$Object$:
	pushl %ebp
	movl %esp, %ebp

/* Return */
	jmp Object$$Object$$exit
Object$$Object$$exit:
	popl %ebp
	ret

/* Method Declaration */
	.align 4
	.globl Object$equals$Object$
Object$equals$Object$:
	pushl %ebp
	movl %esp, %ebp

/* This */
	movl 8(%ebp), %eax
	pushl %eax

/* Parameter */
	leal 12(%ebp), %eax
	pushl %eax

/* Deref */
	popl %eax
	movl (%eax), %eax
	pushl %eax

/* Binary Operation: Equal */
	popl %eax
	popl %edx
	cmpl %eax, %edx
	sete %al
	movzbl %al, %eax
	pushl %eax

/* Return */
	popl %eax
	jmp Object$equals$Object$$exit
Object$equals$Object$$exit:
	popl %ebp
	ret
