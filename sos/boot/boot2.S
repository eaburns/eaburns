/* In this file, we have the disk geometry, however we need
 * to load the kernel into memory and then switch to 
 * protected mode and jump the kernel. */
.text
.code16

#define ADDR(x) (x-_start)

/* Where we load the kernel in memory. */
#define KERNEL_SEGMENT 0x50
#define KERNEL_ADDRESS 0x500

/* Protected mode stack address. */
#define PROT_STACK_ADDR 0x9FBFF

/* Ammount of bytes to reserve for kernel parameters. */
#define PARAM_LEN 12

/* 
 * File system information. 
 */

/* Max length of an EFS file name. */
#define EFS_FILE_NAME_LEN 0xC

/* Number of access block pointers for each type. */
#define DIRECT_BLKS 0x5
#define INDIR_BLKS (DIRECT_BLKS+0x100)

/* EFS block size. */
#define BLOCK_SIZE 1024 /* Bytes. */

/* The EFS Superblock is on sector 1 of the disk. */
#define EFS_SUPERBLOCK 0x1

/* The size of an EFS inode. */
#define EFS_INODE_LEN 0x40
#define INODE_PER_BLOCK (BLOCK_SIZE/EFS_INODE_LEN)

.globl _start
_start:
  jmp begin

sectors:
  .word 0
heads:
  .word 0
cylinders:
  .word 0

boot_disk:
  .byte 0

not_impl_str:
  .string "Double indirect block pointers not yet implemented.\n\r"
found_str:
  .string "Kernel found.\n\r"
not_found_str:
  .string " not found.\n\r"
stage2_str:
  .string "Stage2 loaded and executing.\n\r"
kernel_loading_str:
  .string "Loading kernel "
error_str:
  .string "Error\n\r"
nl:
  .string "\n\r"
prompt_str:
  .string "boot> "

press_ent_str:
  .string "Press enter to boot default kernel: "
default_kernel_file:
  .string "kernel"
default_kernel_file_end:

default_kernel_param:
  .string "init=init"
default_kernel_param_end:

/* Input buffer. */
inbuff:
  . = . + 50

/* bytes for our kernel file name. */
kernel_file:
  . = . + EFS_FILE_NAME_LEN

/* Reserve bytes for kernel parameters. */
kernel_param:
  . = . + PARAM_LEN

mem_size:
  .word 0

begin:

  pushw %ds

  cli
  movw %bx, %es
  movw %bx, %ds
  sti

  movw $ADDR(boot_disk), %di
  movb %dl, (%di)

  cli
  popw %ds
  sti

  /* Copy over the disk geometry information. */
  movw $ADDR(sectors), %di
  movw $0x3, %cx
  rep
  movsw
  
  cli
  movw %bx, %ds
  sti

  movw $ADDR(stage2_str), %si
  call print

  movw $ADDR(nl), %si 
  call print
  movw $ADDR(nl), %si 
  call print
  movw $ADDR(press_ent_str), %si
  call print
  movw $ADDR(default_kernel_file), %si
  call print
  movw $ADDR(nl), %si 
  call print

#ifdef __NOPROMPT__
  jmp default_kern_name
#endif /* __NOPROMPT__ */

prompt:

  /* Prompt user for kernel name. */
  movw $ADDR(prompt_str), %si
  call print

  movw $ADDR(inbuff), %di
  movw $(EFS_FILE_NAME_LEN+PARAM_LEN), %cx
  call getstr

  movw $ADDR(inbuff), %si
  movb (%si), %al
  orb %al, %al
  jz default_kern_name 

  movw $ADDR(inbuff), %si

kern_name_copy:
  movw $ADDR(inbuff), %si
  movw $ADDR(kernel_file), %di
copy_kern_loop:
  lodsb
  cmpb $0x20, %al
  je ld_param
  stosb
  orb %al, %al
  jnz copy_kern_loop
  jmp got_kernel_name

ld_param:
  movw $ADDR(kernel_param), %di
ld_param_loop:
  lodsb
  stosb
  orb %al, %al
  jnz ld_param_loop
  jmp got_kernel_name

default_kern_name:
  /* Here, the user hit enter for the default kernel
   * name.  Lets copy it into the buffer for them. */
  movw $ADDR(default_kernel_file), %si
  movw $ADDR(kernel_file), %di
  movw $(default_kernel_file_end - default_kernel_file), %cx
  cld
  rep
  movsb

  /* Copy over default parameters. */
  movw $ADDR(default_kernel_param), %si
  movw $ADDR(kernel_param), %di
  movw $(default_kernel_param_end - default_kernel_param), %cx
  cld
  rep
  movsb
  
got_kernel_name:

  /* Read the superblock off of the disk. */
  movw $(EFS_SUPERBLOCK), %ax
  movw $ADDR(efs_superblock), %bx
  call read_sector 

  movw $ADDR(efs_superblock), %si
  movw 4(%si), %ax   /* First inode block number. */
  movw $ADDR(efs_buffer), %bx
  call read_fs_block

  /* Copy over inode info to the buffer. */
  movw $ADDR(efs_buffer), %si
  movw $ADDR(efs_inode), %di
  movw $0x9, %cx
  rep
  movsw
  
  /* Get the last logical block number of the root dir. */
  movw $ADDR(efs_inode), %si
  movw (%si), %ax
  xorw %dx, %dx
  movw $BLOCK_SIZE, %bx
  divw %bx
  movw $ADDR(last_block), %di
  stosw

  /* Check the root directory for the kernel file. */
  xorw %cx, %cx /* Starting at logical file block 0. */

  /* bp keeps track of the number of bytes searched so far
   * this way we can be sure to not check more bytes than the file for 
   * the root directory contains. */
  xorw %bp, %bp   /* Bytes searched. */

find_kernel:

  /* Read a root dir block into our memory buffer. */
  movw %cx, %ax
  call get_bnum
  movw $ADDR(efs_buffer), %bx
  call read_fs_block 

  /* Save our logical block number. */
  pushw %cx

  /* Search our buffer for the kernel file's name. */
  movw $ADDR(efs_buffer), %si
search_block: 
  movw $ADDR(kernel_file), %di

  /* Push the file's inode number on the stack. */
  movw 12(%si), %ax
  pushw %ax

  /* Check if this is our file. */
  call strcmp
  orw %ax, %ax
  jz kern_found

  /* If this is not our file, get ready to read the next
   * file from the dir. */
  popw %ax

  /* Inc number of bytes read and check against the inode. */
  pushw %si
  addw $0x10, %bp 
  movw $ADDR(efs_inode), %si         
  cmpw (%si), %bp
  je not_found
  popw %si
  
  addw $0x10, %si
  movw $ADDR(efs_buffer), %bx
  addw $BLOCK_SIZE, %bx
  cmpw %bx, %si
  je search_fail
  jmp search_block
   
/* The file wasn't found in this current block, load another if there
 * is one. */
search_fail:
  popw %cx
  incw %cx
  movw $ADDR(last_block), %si
  movw (%si), %bx
  cmpw %bx, %cx
  jle find_kernel
not_found:
  movw $ADDR(kernel_file), %si
  call print
  movw $ADDR(not_found_str), %si
  call print

  jmp prompt
 
/* Kernel is found, and its inode number is on the stack. */
kern_found:
  movw $ADDR(found_str), %si 
  call print

/* Here we load the kernel's inode into our buffer. */
  popw %cx
  movw %cx, %ax
  call read_inode

  movw $ADDR(kernel_loading_str), %si
  call print

  movw $ADDR(kernel_file), %si
  call print

  /* Get and store the last logical file block number. */
  movw $ADDR(efs_inode), %si
  movw $ADDR(last_block), %di
  movw (%si), %ax
  movw $BLOCK_SIZE, %bx
  xorw %dx, %dx
  divw %bx
  stosw

  /* LOAD KERNEL TO MEMORY. */
  xorw %cx, %cx   /* File logical block 0. */
  movw $KERNEL_SEGMENT, %bx
  movw %bx, %es
  xorw %bx, %bx
read_kern_loop:
  pushw %cx
  pushw %bx

  movw $0x0E2E, %ax       /* BIOS teletype */
  movw $0x0007, %bx       /* page 0, attribute=7 */
  int $0x10

  movw %cx, %ax 
  call get_bnum
  popw %bx
  pushw %bx
  call read_fs_block

  /* Move our memory destination. */
  popw %bx
  addw $BLOCK_SIZE, %bx

  /* Next logical sector. */
  popw %cx
  incw %cx
  
  movw $ADDR(last_block), %si
  cmpw (%si), %cx
  jle read_kern_loop

  /* If we have a floppy, stop it from spinning. */
  testb $0x80, %dl
  jnz _no_floppy

  movw  $0x3F2, %dx
  xorb  %al, %al
  outb  %al, %dx

_no_floppy:

  call a20_enable

  call get_memsize
  movw $ADDR(mem_size), %di
  movw %ax, (%di)

  /* here we switch to pmode. */  
  DATA32 call pmode_switch    /* This call does not return
                               * as you expect it to. */
  /* WE WILL NEVER GET HERE. */

error:
  movw $ADDR(error_str), %si
  call print

/* Reboot. */
  movw $0x0472, %di
  movw %ax, (%di)
  ljmp $0xFFFF, $0x0000


/******************************************************************************
 * END OF MAIN CODE.
 *****************************************************************************/

/* Print the size of extended memory. */
get_memsize:
  movb $0x88, %ah
  int $0x15
  ret


/* Flush the kbd controller. */
kbd_flush:
  inb $0x64
  testb $0x2, %al
  jnz kbd_flush
  ret
  
/*
 * a20_enable
 *  Enables the A20 gate, using the keyboard controller.
 */
a20_enable:
  cli
  
  call    kbd_flush
  mov     $0xd1, %al
  outb    $0x64
  call    kbd_flush
  mov     $0xdf, %al
  outb    $0x60
  call    kbd_flush

  sti
  ret

/*
 * getstr
 *  Gets character input from the keyboard and outputs the input character to
 *  the screen and it copys it into the buffer es:di.  A carrage return 
 *  terminates input.  Also make sure you have an extra byte for the '\0'
 *  Entry: cx contains max number of characters to input, es:di is the 
 *         destination buffer of the string.
 */

getstr:
  xorw %dx, %dx   /* Character count. */
  jmp getstr_begin

back_space:
  /* Don't let the user backspace past the beginning
   * of their input. */
  orw %dx, %dx
  jnz chars_entered

  /* Echo a space. */
  movw $0x0E20, %ax
  movw $0x0007, %bx
  int $0x10
  jmp getstr_loop  

chars_entered:
  subw $0x1, %di
  incw %cx
  decw %dx
  
getstr_loop:
  call getchar

  /* Carrage return check. */
  cmpb $0xD, %al
  je getstr_done

  /* Backspace check. */
  cmpb $0x8, %al
  je back_space

  stosb 
  decw %cx
  incw %dx

getstr_begin:
  orw %cx, %cx
  jnz getstr_loop

getstr_done:

  /* Null terminate. */
  xorb %al, %al
  stosb

  /* Output a newline. */
  movw $0x0E0A, %ax       /* BIOS teletype */
  movw $0x0007, %bx       /* page 0, attribute=7 */
  int $0x10

  ret

/* 
 * getchar
 *  Gets a single character input from the keyboard and outputs it on the 
 *  display, the ascii code is returned via al for the input character.
 *  Trashed: AX, BX
 */
getchar:
  /* Get character. */
  xorb %ah, %ah
  int $0x16
  
  /* Echo character. */
  movb $0x0E, %ah         /* BIOS teletype */
  movw $0x0007, %bx       /* page 0, attribute=7 */
  int $0x10

  cmpb $0x8, %al
  jne getchar_done

  /* If we have a backspace, overwrite the
   * last character before returning. */  
  movw $0x0E20, %ax
  movw $0x0007, %bx
  int $0x10
  movw $0x0E08, %ax
  movw $0x0007, %bx
  int $0x10
  
  movb $0x8, %al
  
getchar_done:
  ret

/*
 * print 
 *   Prints a string that is in ds:si to the screen
 */
_print_:
  movb $0x0E, %ah         /* BIOS teletype */
  movw $0x0007, %bx       /* page 0, attribute=7 */
  int $0x10

print:
  lodsb
  orb %al, %al
  jnz _print_
  ret
  
/*
 * get_bnum
 *  Gets the filesystem logical block number from a FILE logical block number
 *  using the inode data, AX has the file logical block number and the filesystem
 *  logical block number is returned via AX.
 */
get_bnum:

  /* Check if we have a direct data block. */
  cmpw $DIRECT_BLKS, %ax
  jl direct_block

  cmpw $INDIR_BLKS, %ax
  jl indirect_block

/* direct_block */
  movw $ADDR(not_impl_str), %si
  call print
  jmp error

indirect_block:

  pushw %ax
  movw $ADDR(efs_superblock), %si
  movw 0xC(%si), %bx
  movw $ADDR(efs_inode), %si
  movw 0x1C(%si), %ax
  addw %bx, %ax 

  pushw %es
  movw $0x900, %bx
  movw %bx, %es
  movw $ADDR(efs_buffer), %bx
  call read_fs_block
  popw %es

  popw %ax
  subw $DIRECT_BLKS, %ax
  movw $0x4, %bx
  mulw %bx
  movw $ADDR(efs_buffer), %si
  addw %ax, %si
  movw (%si), %ax
  jmp _add_doff

direct_block:
  movw $ADDR(efs_inode_datablks), %si
  movw $0x4, %bx
  mulw %bx
  addw %ax, %si
  movw (%si), %ax

_add_doff:
  movw $ADDR(efs_superblock), %si
  addw 12(%si), %ax
  
  ret

/*
 * read_fs_block
 *  This function reads a file system block from the disk given the
 *  logical block number in AX and the dest buffer of ES:BX.
 */
read_fs_block:
  /* Disk_block = (fs_block * 2) */
  movw $0x2, %cx
  mulw %cx

  /* Read 2 sectors from the disk. */
  pushw %bx
  pushw %ax
  call read_sector
  popw %ax
  popw %bx

  incw %ax
  addw $0x200, %bx
  call read_sector

  ret

/* 
 * read_inode
 *  Reads an inode from the disk given the inode number in AX.
 *  The inode is placed in the efs_inode buffer. 
 */
read_inode:

  /* Find the inode block. */
  xorw %dx, %dx
  movw $INODE_PER_BLOCK, %bx
  divw %bx
  pushw %dx

  /* Read the inode block. */
  movw $ADDR(efs_superblock), %si
  addw 4(%si), %ax
  movw $ADDR(efs_buffer), %bx
  call read_fs_block

  /* Copy our inode into the buffer. */
  popw %dx
  movw $EFS_INODE_LEN, %ax
  mulw %dx
  movw $ADDR(efs_buffer), %si
  movw $ADDR(efs_inode), %di
  addw %ax, %si
  movw $EFS_INODE_LEN, %cx
  rep
  movsb

  ret

/*
 * read_sector
 *  Reads a sector off of the disk given the logical sector value.
 *  Logical sector value is in AX into ES:BX
 * Trashes: DI, AX, DX, SI 
 */
_head:
  .word 0
_cylinder:
  .word 0
_sector:
  .word 0

read_sector:

  /* reset disk controller */
  pushw %bx
  pushw %ax
  xorw %ax, %ax
  int $0x13
  popw %ax

  movw $ADDR(_head), %di
  movw $ADDR(sectors), %si

  /* LBA to CHS 
   * sector = (LBA%sectPerTrack)+1
   * cylinder = (LBA/sectPerTrack)/NumHeads
   * head = (LBA/sectPerTrack)%NumHeads
   */

  movw (%si),%bx
  xorw %dx, %dx
  divw %bx
  /* Here: 
   * dx = LBA%sectPerTrack
   * ax = LBA/sectPerTrack */

  incw %dx
  movw %dx, 4(%di) /* Store sector number. */
  
  movw 2(%si), %bx
  xorw %dx, %dx
  divw %bx
  
  movw %ax, 2(%di) /* Save cylinder number. */
  movw %dx, (%di)  /* Save head number. */

  /* Setup geometry to read */

  /* Setup head. */
  movw $ADDR(boot_disk), %si
  movb (%si), %dl
  movw $ADDR(_head), %si
  movw (%si), %cx
  movb %cl, %dh

  /* setup cylinders */
  movw 2(%si), %ax
  movb %al, %ch
  movb %ah, %cl
  orw 4(%si), %cx

  movb $0x1, %al  
  movb $0x2, %ah
  popw %bx
  int $0x13
  jc error
  
  ret

/* 
 * strcmp
 *  Compairs 2 strings from DS:SI and ES:DI and returns 0 via AX if they are equal
 */
_strcmp:
  orb %bh, %bh
  jz strcmp_succeed

  incw %si
  incw %di
strcmp:
  movb (%si), %bh
  movb (%di), %bl

  cmpb %bh, %bl
  je _strcmp

strcmp_fail:
  movw $0x1, %ax
  ret

strcmp_succeed:
  movw $0x0, %ax
  ret
  

/*
 * Switches to protected mode from real mode.
 */
pmode_switch:
  cli

  /* Grab our return address. */
  popl %ebx


  /* Load global descriptor table. */
  DATA32 ADDR32 lgdt ADDR(gdt_d)

  /* Set the protected mode bit in cr0 to enable pmode. */
  movl %cr0, %eax
  orl $0x1, %eax
  movl %eax, %cr0
  
  DATA32 ljmp $0x8, $clear_pipeline

  .code32
clear_pipeline:

  /* Setup segment registers. */
  movw $0x10, %ax 
  movw %ax, %ds
  movw %ax, %ss
  movw %ax, %es
  movw %ax, %fs
  movw %ax, %gs

  /* Save our return address, setup the new stack and
   * put our return address back on the new stack. */
  movl $PROT_STACK_ADDR, %esp
  movl $PROT_STACK_ADDR, %ebp

  /*
   * Setup the protected mode stack.
   */

  pushl $kernel_param /* Push on the kernel arguments. */

  /* Put the memory size on the p-mode stack. */
  xorl %eax, %eax
  movl $mem_size, %esi
  lodsw 
  pushl %eax

  pushl $KERNEL_ADDRESS /* Return to the kernel. */

  movl $KERNEL_ADDRESS, %ebx

  ret
  .code16

/* Global descriptor table. */
  .p2align 2    /* Force 4-byte allignment. */
gdt:
  /* Reserved segment. */
  .word 0, 0
  .byte 0, 0, 0, 0

  /* Code segment. */
  .word 0xFFFF, 0
  .byte 0, 0x9A, 0xCF, 0

  /* Data segment. */
  .word 0xFFFF, 0
  .byte 0, 0x92, 0xCF, 0
gdt_end:

/* Tells the cpu where the gdt is. */
gdt_d:
  .word gdt_end-gdt-1
  .long gdt

/* This is the info from the superblock. */
efs_superblock:
  .byte 0, 0, 0, 0 /* Ident. */ 
  .long 0 /* Inode offset */
  .long 0 /* Inode count */
  .long 0 /* Data offset */
  .long 0 /* Data count */

/* This is all the data we need to know from the inode. */
efs_inode:
  .long 0 /* Size */
  .long 0 /* Refs */
efs_inode_datablks:
  .long 0, 0, 0, 0, 0 /* Data blocks */
  .long 0 /* Indir data */
  .long 0 /* Double Indir data */

/* Number of blocks in the root directory. */
last_block:
  .word 0

/* Marks the beginning of the buffer for a file system block. */
efs_buffer:
