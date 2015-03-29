#include <cmos.h>
#include <bus.h>
#include <ktypes.h>

static uint32 iot = I386_BUS_IO_T;

/* CMOS index data port. */
static uint32 io_index = 0x70;
/* CMOS requested data port. */
static uint32 io_data = 0x71;

/* Returns data from the given CMOS index. */
uint32 cmos_get_data( uint32 i )
{
  uint8 index = 0xFF&i;
  uint32 retval;

  bus_space_write_8( iot, io_index, 0, index );
  bus_space_read_8( iot, io_data, 0, retval );

  return retval;
}
