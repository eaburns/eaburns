#include <irq.h>
#include <ktypes.h>
#include <retval.h>
#include <kstdlib.h>

/* All interrupts are null for now. */
intcallback_t irqs[MAX_IRQ_NUM] = 
 { NULL,NULL,NULL,NULL,NULL,NULL,NULL,
   NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};

uint32 bind_irq( uint16 num, void *handler )
{
  irqs[num] = handler;

  return RET_SUCCESS;
}
