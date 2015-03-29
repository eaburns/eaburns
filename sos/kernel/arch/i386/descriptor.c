#include <descriptor.h>
#include <ktypes.h>

void set_interrupt( interrupt_descriptor *id, void *funct, uint32 selector, 
                    uint32 type, uint32 dpl )
{
  id->offset_low = (uint32)funct;
  id->selector = (uint16)selector;
  id->dword_count = 0;
  id->zero0 = 0;
  id->type = type;
  id->zero1 = 0;
  id->dpl = dpl;
  id->p = 1;
  id->offset_high = ((uint32)funct)>>16;
}


void set_selector( global_descriptor *desc, uint32 base, uint32 limit, 
                   uint32 type, uint32 dpl, uint32 opsize,
                   uint32 gran )
{
    desc->limit_low = limit;
    desc->base_low = base;
    desc->type = type;
    desc->dpl = dpl;
    desc->p = 1;
    desc->limit_high = limit >> 16;
    desc->avl = 0;
    desc->zero = 0;
    desc->opsize = opsize;
    desc->granularity = gran;
    desc->base_high = base >> 24;
}
