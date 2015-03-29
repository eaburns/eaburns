#ifndef _REGS_H_
#define _REGS_H_

#include <ktypes.h>

/* The defines in here are for ASM access to struct members. */
struct regs {
  uint32 edi __attribute__((packed)); 
  uint32 esi __attribute__((packed));
  uint32 ebp __attribute__((packed)); 
  uint32 edx __attribute__((packed)); 
  uint32 ebx __attribute__((packed)); 
  uint32 ecx __attribute__((packed));
  uint32 eax __attribute__((packed));
};

#endif /* _REGS_H_ */
