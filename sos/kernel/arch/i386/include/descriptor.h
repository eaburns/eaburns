#ifndef _DESCRIPTOR_H_
#define _DESCRIPTOR_H_

#include <ktypes.h>

/* I guess this goes here... */
/* The kernel code selector in the GDT. */
#define KCODE_SELECTOR 0x8
#define KDATA_SELECTOR 0x10
#define UCODE_SELECTOR 0x18
#define UDATA_SELECTOR 0x20
#define TSS_SELECTOR   0x28

/* Descriptor protection levels. */
#define DPL_KERNEL 0
#define DPL_USER 3

/*  
 * A table_descriptor describes where a table (GDT, IDT, LDT) is in memory.  
 */
typedef struct {
  /* Length in bytes of the table. */
  unsigned limit:16 __attribute((packed));

  /* Base address of the table. */
  unsigned base:32 __attribute((packed));

} table_descriptor;

/* 
 * An entry into the IDT. 
 */
typedef struct {
  /* Low 16 bits of the callback function. */
  unsigned offset_low:16 __attribute((packed)); 

  /* The segment selector of the callback. */
  unsigned selector:16 __attribute((packed));

  /* Must be zero for entries in the IDT. */
  unsigned dword_count:5 __attribute((packed));

  /* 3 bits of zeroes. */
  unsigned zero0:3 __attribute((packed));

  /* Type of IDT entry. */
  unsigned type:4 __attribute((packed));

  /* 1 bit of zero. */
  unsigned zero1:1 __attribute((packed));

  /* DPL must be set to hardware or software. */
  unsigned dpl:2 __attribute((packed));

  /* Interrupt function is present in memory. */
  unsigned p:1 __attribute((packed));

  /* High 16 bits of the callback. */
  unsigned offset_high:16 __attribute((packed));

} interrupt_descriptor;

/* Gate type is either an interrupt gate or a trap gate. */
#define DESC_TYPE_INTR_GATE 14
#define DESC_TYPE_TRAP_GATE 15

/* An entry into the global descriptor table. */
typedef struct {
  /* Low 16 bits of the limit. */
  unsigned limit_low:16       __attribute((packed));

  /* Low 16 bits of the base address. */
  unsigned base_low:24        __attribute((packed));

  /* Type (read/write/execute)? */
  unsigned type:5             __attribute((packed));

  /* Privledge level. */ 
  unsigned dpl:2              __attribute((packed));

  /* Segment present bit. */
  unsigned p:1                __attribute((packed));
  
  /* High 3 bits of the limit. */
  unsigned limit_high:3      __attribute((packed));

  /* Available for the OS. */
  unsigned avl:2              __attribute((packed));

  /* Zero */
  unsigned zero:1             __attribute((packed));

  /* Default op size for code descriptors. */
  unsigned opsize:1           __attribute((packed));

  /* Granularity, 1 = limit*0x1000, 0 = limit */
  unsigned granularity:1      __attribute((packed)); 

  /* High 16 bits of the base address. */
  unsigned base_high:8       __attribute((packed));

} global_descriptor;

/* Sets an interrupt descriptor with the given information. */
void set_interrupt( interrupt_descriptor *id, void *funct, uint32 selector, 
                    uint32 type, uint32 dpl );

/* Sets a global descriptor table entry. */
void set_selector( global_descriptor *desc, uint32 base, uint32 limit, 
                   uint32 type, uint32 dpl, uint32 opsize,
                   uint32 gran );

#define GDT_ER 27 /* Execute and read. */
#define GDT_RW 19 /* Read and write. */

#define __set_selector( desc, base, limit, type, dpl ) \
  set_selector( desc, base, limit, type, dpl, 1, 1 )
    
#endif /* _DESCRIPTOR_H_ */
