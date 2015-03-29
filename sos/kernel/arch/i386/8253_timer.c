#include <8253_timer.h>
#include <bus.h>
#include <ktypes.h>
#include <retval.h>
#include <irq.h>
#include <kstdio.h>
#include <timer.h>

/*
 * NOTE: I need to look into this timer stuff more!!!!
 */
static uint32 iot = I386_BUS_IO_T;

static uint32 io_command = 0x43;
static uint32 io_data = 0x40;

/* Initialize the 8253 timer chip. */
int init_timer()
{
  uint32 period = 11931; /* Everyone seems to use this value.... */

  /* channel 0, LSB/MSB, mode3, binary.  <--- look this up. */
  bus_space_write_8( iot, io_command, 0, 0x36 );

  bus_space_write_8( iot, io_data, 0, period&0xFF );
  bus_space_write_8( iot, io_data, 0, (period>>8)&0xFF );

  if( bind_irq( 0, timer_interrupt ) == RET_FAILURE ) {
    kprintf( "timer: failed to bind irq 0.\n" );
    return RET_FAILURE;
  }

  return RET_SUCCESS;
}
