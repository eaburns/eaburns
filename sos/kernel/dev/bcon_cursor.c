#include <bcon_cursor.h>
#include <bcon_arch.h>
#include <ktypes.h>

void __inline bcon_cursor_up()
{
  uint32 x, y;
  bcon_get_cursor_pos( &x, &y );
  bcon_set_cursor_pos( x, y-1 );
}

void __inline bcon_cursor_down()
{
  uint32 x, y;
  bcon_get_cursor_pos( &x, &y );
  bcon_set_cursor_pos( x, y+1 );
}

void __inline bcon_cursor_left()
{
  uint32 x, y;
  bcon_get_cursor_pos( &x, &y );
  bcon_set_cursor_pos( x-1, y );
}

void __inline bcon_cursor_right()
{
  uint32 x, y;
  bcon_get_cursor_pos( &x, &y );
  bcon_set_cursor_pos( x+1, y );
}

void __inline bcon_cursor_home_line()
{
  uint32 x, y;
  bcon_get_cursor_pos( &x, &y );
  bcon_set_cursor_pos( 0, y );
}

uint8 __inline bcon_cursor_off_end()
{
  uint32 x, y;
  bcon_get_cursor_pos( &x, &y );

  if( x >= bcon_get_term_cols() || y >= bcon_get_term_rows() )
    return 1;

  return 0;
}
