#ifndef _TSS_H_
#define _TSS_H_

#define TSS_SIZE 104

typedef struct {
  uint32 prev_task;
  uint32 esp0;
  uint32 ss0;
  uint32 esp1;
  uint32 ss1;
  uint32 esp2;
  uint32 ss2;
  uint32 cr3;
  uint32 eip;
  uint32 eflags;
  uint32 eax, ecx, edx, ebx;
  uint32 esp, ebp, esi, edi;
  uint32 es;
  uint32 cs;
  uint32 ss;
  uint32 ds;
  uint32 fs;
  uint32 gs;
  uint32 ldt;
  uint16 trace, bitmap;
} tss_t;

#endif /* _TSS_H_ */
