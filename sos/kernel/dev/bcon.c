#include <bcon.h>
#include <bcon_arch.h>
#include <bcon_cursor.h>
#include <kbd.h>
#include <ktypes.h>
#include <memutil.h>
#include <device.h>
#include <init.h>
#include <fs.h>

#include <config/kbd.h>
#ifdef CONFIG_KBD
#include <bcon_kbd.h>
#endif /* CONFIG_KBD */

uint16 bcon_attrs = TERM_ATTR;

static device_t bcon_dev;

static int bcon_fs_write( file_t *file, void *buff, size_t len );

int bcon_init()
{
  ONLY_ONCE();
  bcon_clear_screen();

#ifdef CONFIG_KBD
  bcon_kbd_init();
#endif /* CONFIG_KBD */
  bcon_dev.name = "bcon";
  bcon_dev.minor = 0;
  bcon_dev.major = MAJOR_CHAR_CONS;
  bzero( &bcon_dev.fops, sizeof(file_ops_t) );
  bcon_dev.fops.write = bcon_fs_write;
  bcon_dev.wait_queue = NULL;

  /* Add the device to the device list. */
  /* device_register( bcon_dev ); */

  return RET_SUCCESS;
}

/* This is called with the rest of the devices after kalloc is initialized. */
static int bcon_init2()
{
  ONLY_ONCE();

  return RET_SUCCESS;
}

init_info( "bcon", bcon_init2 );

static int bcon_fs_write( file_t *file, void *buff, size_t len )
{
  return -1;
}

uint32 bcon_write( uint8 *buff, uint32 len )
{
  int i;
  
  for( i = 0; i < len; i++ )
    bcon_putchar( buff[i] );

  return len;
}

void bcon_clear_screen()
{
  int i;

  for( i = 0; i < bcon_get_term_rows(); i++ )
    bcon_clear_line( i );

  bcon_set_cursor_pos( 0, 0 ); 
}

void bcon_putchar( int c )
{
  switch( c ) {

  /* Either form of the newline. */
  case '\r':
    bcon_cursor_home_line(); break;

  case '\n':
    /* Goto the next line. */
    bcon_cursor_down();
    bcon_cursor_home_line();

    /* Check if we need to scroll up. */
    if( bcon_cursor_off_end() == 1 )
      bcon_scroll_screen_up();

    break;

  /* Tab is just 4 spaces. */
  case '\t':
    bcon_putchar( ' ' );
    bcon_putchar( ' ' );
    bcon_putchar( ' ' );
    bcon_putchar( ' ' );
    break;

  /* A backspace moves the cursor back, draws a blank, and moves the cursor
   * back again. */
  case '\b':
      bcon_cursor_left(); bcon_putchar( ' ' ); bcon_cursor_left();
      break;

  /* Other than a few controll characters, we just wanna write the character
   * to the screen. */
  default:
    /* Write a character and move the cursor. */
    bcon_write_char( c, bcon_attrs );

    /* Check if we need to scroll up. */
    if( bcon_cursor_off_end() == 1 )
      bcon_scroll_screen_up();
  }
}
