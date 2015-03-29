/*
 * Simple boot loader
 * 2005 Ethan Burns <eaburns@cisunix.unh.edu>
 */
#define ADDR(x) (x-_start)

#define STAGE2_LOAD_SEG 0x900
#define STAGE2_LOAD_ADDR 0x9000

.text
.code16	/* 16 bit code */

.globl _start
_start:
  jmp begin 

/*
 * globals
 */

/* Logical sector of stage2 code */
stage2_sector:
  .word 2874
/* Number of sectors for stage 2 */
stage2_size:
  .byte 4

/* Disk geometry */
sectors:
  .word 0
heads:
 .word 0
cylinders:
  .word 0

/* The boot disk. */
boot_disk:
  .byte 0

read_error_str:
  .string "Read "
error_str:
  .string "Error\n\r"
probing_str:
  .string "Probing"
loading_stage2_str:
  .string "\n\rLoading Stage2"
chs_str:
  .string "C/H/S "
nl:
  .string "\n\r"

/*
 * start, the main routine for the boot loader.
 */ 
begin:

  cli
  /* This log jump fixes some flakie BIOSs */
  ljmp $0x0, $(real_begin-_start+0x7C00)

real_begin:

  /* Setup the segment registers */
  movw $0x07C0, %ax
  movw %ax, %ds
  movw %ax, %es
  movw %ax, %ss
  movw $0x2000, %sp
  sti

  /* Save boot disk number. */
  movw $ADDR(boot_disk), %di
  movb %dl, (%di)

  /* Grab disk geometry. */
  movb $0x8, %ah
  int $0x13
  jc error

  /* Fix segment register that int 0x13 messes up */
  movw $0x07C0, %ax
  movw %ax, %es

  movw $ADDR(sectors), %di

  /* Save number of heads */
  xorw %ax, %ax
  movb %dh, %al
  incw %ax
  movw %ax, 2(%di)

  xorw %dx, %dx
  movb %cl, %dl
  shlw $0x2, %dx
  movb %ch, %al
  movb %dh, %ah

  /* Save number of cylinders */
  incw %ax
  movw %ax, 4(%di)

  xorw %ax, %ax
  movb %dl, %al
  shrb $0x2, %al
  /* Save number of sectors */
  movw %ax, (%di)
  
  /* Restore boot disk number. */
  movw $ADDR(boot_disk), %si
  movb (%si), %dl
  
  /* If we have a floppy disk, assume we need to probe for the sector number. */
  testb $0x80, %dl
  jnz _no_floppy_probe

  call floppy_probe

  xorb %ch, %ch
  movw $ADDR(sectors), %di
  movw %cx, (%di)

_no_floppy_probe:

  movw $ADDR(chs_str), %si
  call print
  movw $ADDR(cylinders), %si
  movw $3, %cx
  std
1:
  movw $10, %bx
  lodsw
  call itoa
  decw %cx
  jz 2f
  movw $0x0E2F, %ax
  movw $0x0007, %bx
  int $0x10
  jmp 1b
2: cld
  
  movw $ADDR(loading_stage2_str), %si
  call print

  movw $ADDR(stage2_sector), %si
  movw (%si), %ax

  movw $STAGE2_LOAD_SEG, %bx
  movw %bx, %es
  xorw %bx, %bx

  /* Get number of sectors to read. */
  xorw %cx, %cx
  movw $ADDR(stage2_size), %si
  movb (%si), %cl
  movw %cx, %bp

_read_loop:
  
  /* Print a period */
  pushw %ax
  pushw %bx
  movw $0x0E2E, %ax       /* BIOS teletype */
  movw $0x0007, %bx       /* page 0, attribute=7 */
  int $0x10
  popw %bx
  popw %ax

  /* Read the sector. */
  pushw %ax
  call read_sector
  popw %ax
  addw $0x200, %bx    /* Next 512 bytes. */
  incw %ax
  decw %bp
  jnz _read_loop

  movw $ADDR(nl), %si
  call print

  /* Stage2 expects si to have the address of the disk
   * disk geometry data.  and dl to have the boot disk number */
  movw $ADDR(boot_disk), %si
  movb (%si), %dl 
  movw $ADDR(sectors), %si
  movw $STAGE2_LOAD_SEG, %bx
  ljmp $0, $STAGE2_LOAD_ADDR

  /* We shouldn't get here. */
  jmp error

read_error:
  movw $ADDR(read_error_str), %si
  call print
error:
  movw $ADDR(error_str), %si
  call print
  jmp .

/*
 * print 
 *   Prints a string that is in ds:si to the screen
 * Trashes: AH, BX, AL
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
  
/* itoa
 * prints number in %ax with radix in %bx
 */
itoa:
  pushw %cx
  xorw %cx, %cx           /* zero counter. */

/* Get digits loop. */
1:
  xorw %dx, %dx           /* clear for division. */
  divw %bx
  pushw %dx               /* Save MOD (our digit). */
  incw %cx
  orw %ax, %ax
  jnz 1b

/* Print digits loop. */
2:
  popw %ax
  addw $0x30, %ax
  cmpw $0x3A, %ax
  jge 3f
  jmp 4f
3: addw $0x6, %ax
4:
  movb $0x0E, %ah
  movw $0x0007, %bx
  int $0x10
  decw %cx
  jnz 2b
  popw %cx
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
 * floppy_probe 
 *    is snuck in where the MBR partition table would be on a HD
 * This is currently not true since aparantly we ALWAYS need to probe since
 * I have been unable to EVER get a correct sector count using BIOS.
 * As stated before, this is okay because the code is small enough to fit in
 * and not interfere with the MBR. 
 */
  . = _start + 446
floppy_probe:
  movw $ADDR(probing_str), %si
  call print

  /* reset disk controller */
  xorw %ax, %ax
  int $0x13

  xorb %cl, %cl           /* zero out our sector counter */
  xorb %ch, %ch           /* cylinder 0  */
  xorb %dh, %dh           /* head 0 */
  movw $0x201, %ax        /* setup disk read for 1 sector */

_count_loop:
  incb %cl                /* inc our count (sector count starts at 1 anyway) */

  /* Print a period */
  movw $0x0E2E, %ax       /* BIOS teletype */
  movw $0x0007, %bx       /* page 0, attribute=7 */
  int $0x10

  /* Read the disk */
  movw $0x7000, %bx       /* where to load the sector we read */
  movw $0x201, %ax        /* setup disk read for 1 sector */
  int $0x13               /* try to read */

  jnc _count_loop         /* if read doesn't fail loop again */

  decb %cl                /* our count is greater by 1 */

  movw $ADDR(nl), %si
  call print

  ret
  
/*
 * Force the last 2 bytes to be 0xAA55
 */
   . = _start + 510
	.word 0xAA55
