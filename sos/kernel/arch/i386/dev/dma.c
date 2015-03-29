#include <dma.h>
#include <bus.h>
#include <ktypes.h>

static uint32 iot = I386_BUS_IO_T;

/* The dma controller mode register. */
static uint32 io_mode = 0x0b;
/* The dma controller mask register. */ 
static uint32 io_mask = 0x0a;
/* Page registers; each channel has it's own page register. */
static uint32 io_page[8] = { 0x87, 0x83, 0x81, 0x82, 0x8F, 0x8B, 0x89, 0x8A };
/* Offset registers; again each channel has it's own. */
static uint32 io_offs[8] = { 0x0, 0x2, 0x4, 0x6, 0xC0, 0xC4, 0xC8, 0xCC };
/* Count registers; to each his/her own. */
static uint32 io_count[8] = { 0x1, 0x3, 0x5, 0x7, 0xC2, 0xC6, 0xCA, 0xCE };
/* Clear selected channel register. (channel selected via the mask reg). */
static uint32 io_clear = 0x0C;

/* Disables an 8bit DMA channel (0-3). */
static void dma_disable_channel_8( uint32 c )
{
  /* Mask off the lower two bits of `c' to get our channel number.  Then 
   * set bit 2 (disable channel bit). */
  uint8 mask = (0x3&c)|0x4; 

  bus_space_write_8( iot, io_mask, 0, mask );
}

/* Enables an 8bit DMA channel (0-3). */
static void dma_enable_channel_8( uint32 c )
{
  /* Mask off the lower two bits of `c' to get our channel number.  Then 
   * clear bit 2 (disable channel bit). */
  uint8 mask = (0x3&c)&(~0x4); 

  bus_space_write_8( iot, io_mask, 0, mask );
}

/* Tell the controller to set the specified 8bit channel to single mode
 * for a write transfer. */
static void dma_set_mode_8( uint32 c, uint32 m )
{
  /* Single mode, write, with autoinitialize, increment address. */
  uint8 mode = (0x3&c)|m;
  bus_space_write_8( iot, io_mode, 0, mode );
}

void dma_set_page_8( uint32 c, uint32 p )
{
  uint8 page = 0xFF&p;
  bus_space_write_8( iot, io_page[c-1], 0, page );
}

/* The channel must be cleared before we call this, we send 2 bytes and a 
 * flip-flop has to be initialzed so the DMA controller knows which byte
 * to expect. */
void dma_set_offset_8( uint32 c, uint32 o )
{
  uint8 chan_low = 0xFF&o;
  uint8 chan_high = 0xFF&(o>>8) ;

  bus_space_write_8( iot, io_offs[c-1], 0, chan_low );
  bus_space_write_8( iot, io_offs[c-1], 0, chan_high );
}

/* The channel must be cleared before this too, for the same reason as 
 * for dma_set_offset_8(). */
void dma_set_count_8( uint32 c, uint32 n )
{
  uint8 count_low = 0xFF&n;
  uint8 count_high = 0xFF&(n>>8) ;

  bus_space_write_8( iot, io_count[c-1], 0, count_low );
  bus_space_write_8( iot, io_count[c-1], 0, count_high );
}

/* Clears the channel that is masked off. */
void dma_clear_cur_chan()
{
  bus_space_write_8( iot, io_clear, 0, 0 );
}

/* Sets up the DMA controller for a transfer on the given channel, to/from
 * the given address, with count bytes in the given mode. */
void dma_program_8( uint32 channel, uint32 addr, uint32 count, uint32 mode )
{
  uint32 page = addr / 0xFFFF;
  uint32 offset =  addr % 0xFFFF;
  
  dma_disable_channel_8( channel );  

  dma_set_mode_8( channel, mode );
  dma_set_page_8( channel, page ); 
  dma_clear_cur_chan(); /* Clear channel to init state for sending offset. */
  dma_set_offset_8( channel, offset );
  dma_clear_cur_chan(); /* Clear channel to init state for sending offset. */
  dma_set_count_8( channel, count );

  dma_enable_channel_8( channel );
}
