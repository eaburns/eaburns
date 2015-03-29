#include <bcon.h>
#include <bcon_arch.h>
#include <bcon_kbd.h>
#include <bcon_cursor.h>
#include <memutil.h>
#include <ktypes.h>
#include <bus.h>
#include <retval.h>

#include <config/kbd.h>
#ifdef CONFIG_KBD
#include <kbd.h>
#endif /* CONFIG_KBD */

#define TERM_COLS 80
#define TERM_LINES 25

/* Cursor bus information. (index port 0x3E4, data port 0x3E5). */ 
#define cur_io_type I386_BUS_IO_T
#define cur_io_base 0x3D4
#define cur_index_off 0
#define cur_data_off 1

/* Bus information for video memory. */
#define vid_io_type I386_BUS_MEM_T
#define vid_io_base 0xB8000

/* Gets the cursor offset from the bus. */
static uint32 __inline _get_cur_offset();

void bcon_clear_line( int y )
{
  int i;

  for( i = 0; i < TERM_COLS*2; i += 2 ) {
    bus_space_write_8( vid_io_type, vid_io_base, (y*TERM_COLS*2)+i, ' ' );
    bus_space_write_8( vid_io_type, vid_io_base, (y*TERM_COLS*2)+i+1, bcon_attrs );
  }

}

void bcon_scroll_screen_up()
{
  bcon_cursor_up();
  bcon_cursor_home_line();

  /* Clear 1 line off the screen so when we memcpy the scroll
   * goes smoother. */
  bcon_clear_line( TERM_LINES );

/* This will be changed soon. */
#define VIDEO_MEMORY (uint8*)vid_io_base
  memcpy( VIDEO_MEMORY, VIDEO_MEMORY+(TERM_COLS*2), 
           TERM_COLS*TERM_LINES*2 );

  bcon_clear_line( TERM_LINES-1 );
}

void __inline bcon_write_char( int c, int attributes )
{
  uint32 offset = _get_cur_offset();

  bus_space_write_8( vid_io_type, vid_io_base, offset*2, (c&0xFF) );
  bus_space_write_8( vid_io_type, vid_io_base, (offset*2)+1, attributes );

  offset++;
  bus_space_write_8( cur_io_type, cur_io_base, cur_index_off, 15 );
  bus_space_write_8( cur_io_type, cur_io_base, cur_data_off, (offset&0x00FF) );
  bus_space_write_8( cur_io_type, cur_io_base, cur_index_off, 14 );
  bus_space_write_8( cur_io_type, cur_io_base, cur_data_off, (offset >> 8) );
}

/* Return the term dimentions. */
uint32 __inline bcon_get_term_rows() { return TERM_LINES; }
uint32 __inline bcon_get_term_cols() { return TERM_COLS; }

void __inline bcon_set_cursor_pos( uint32 x, uint32 y )
{
  uint32 offset = x+(y*TERM_COLS);

  bus_space_write_8( cur_io_type, cur_io_base, cur_index_off, 15 );
  bus_space_write_8( cur_io_type, cur_io_base, cur_data_off, (offset&0x00FF) );
  bus_space_write_8( cur_io_type, cur_io_base, cur_index_off, 14 );
  bus_space_write_8( cur_io_type, cur_io_base, cur_data_off, (offset >> 8) );
}

void __inline bcon_get_cursor_pos( uint32* x, uint32* y )
{
  uint32 offset = _get_cur_offset();

  *y = offset/bcon_get_term_cols();
  *x = offset%bcon_get_term_cols();
}

#define I386_KBD_PORT (void*)0x60

/* Initialize the bconsole keyboard. */
uint32 bcon_kbd_arch_init()
{
#ifdef CONFIG_KBD
  return kbd_attach( I386_BUS_IO_T, I386_KBD_PORT, 0, bcon_kbd_handle );
#else
  return RET_SUCCESS;
#endif /* CONFIG_KBD */
}

/*
 * STATIC FUNCTIONS
 */

static uint32 __inline _get_cur_offset()
{
  uint32 offset, offset_h, offset_l;

  bus_space_write_8( cur_io_type, cur_io_base, cur_index_off, 14 );
  bus_space_read_8( cur_io_type, cur_io_base, cur_data_off, offset_h );
  offset_h <<= 8;
  bus_space_write_8( cur_io_type, cur_io_base, cur_index_off, 15 );
  bus_space_read_8( cur_io_type, cur_io_base, cur_data_off, offset_l );
  offset = offset_h|offset_l;

  return offset;
}

