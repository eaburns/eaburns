#include <kbd.h>
#include <init.h>
#include <irq.h>
#include <kstdlib.h>
#include <ktypes.h>
#include <device.h>
#include <bus.h>
#include <retval.h>

#define KBD_DEV_NAME "kbd"

/* Are we initialized? */
static uint8 inited = 0;

/* The key handler. */
static kbd_handler_t handler = NULL;

/* Keyboard's I/O port on the buss. */
static void *kbd_io_base = NULL;
static uint32 kbd_io_off = 0;
static uint32 kbd_io_type = 0;
static uint8 attached = 0;

int kbd_attach( uint32 type, void *base, uint32 off, kbd_handler_t h )
{
  kbd_io_base = base;
  kbd_io_off = off;
  kbd_io_type = type;
  handler = h;
  attached = 1;

  return RET_SUCCESS;
}

int kbd_init()
{
  ONLY_ONCE(); /* Only allow this to init once. */
  inited = 1;

  /* Bind irq 1 and enable. */
  bind_irq( 1, kbd_interrupt );
  kprintf( "%s: bound irq 1\n", KBD_DEV_NAME );
  kbd_enable();

  return RET_SUCCESS;  
}

void kbd_enable()
{
  if( inited == 1 )
    enable_irq( 1 );
}

void kbd_disable()
{
  disable_irq( 1 );
}

int kbd_interrupt()
{
  uint32 key;

  /* Check if we are attached. */
  if( attached == 0 ) { return RET_FAILURE; }

  bus_space_read_8( kbd_io_type, kbd_io_base, kbd_io_off, key );

  /* Call the handler. */
  if( handler != NULL )
    handler( key&(~0x80), ((key&0x80) == 0)?0:1 );

  return RET_SUCCESS;
}


init_info( KBD_DEV_NAME, kbd_init );
