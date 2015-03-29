#include <config/kbd.h>
#ifdef CONFIG_KBD

#include <bcon.h>
#include <bcon_kbd.h>
#include <bcon_arch.h>
#include <config/bcon.h>
#include <kstdio.h>
#include <kbd.h>
#include <retval.h>

/* KEYMAPS. */
uint16 keymaps[][256] = {
  /* us keymap */ 
  { 0, 27,'1','2','3','4','5','6','7','8','9','0','-','=', 8, 9,
   'q','w','e','r','t','y','u','i','o','p','[',']', 13, K_CTL,'a','s',
   'd','f','g','h','j','k','l',';','\'','`', K_LSHIFT, '\\','z','x','c','v',
   'b', 'n','m',',','.','/', K_RSHIFT,'*', K_LMETA,' ', K_CAPS, 0, 0, 0, 0, 0, 
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
     0, 0, 0, K_DEL, 0, K_NUM, K_SCROLL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

                        /* Shifted keys. */
    0, 27,'!','@','#','$','%','^','&','*','(',')','_','+', 8, 9,
   'Q','W','E','R','T','Y','U','I','O','P','{','}', 13, K_CTL,'A','S',
   'D','F','G','H','J','K','L',':','\"','~', K_LSHIFT, '|','Z','X','C','V',
   'B', 'N','M','<','>','?', K_RSHIFT,'*', K_LMETA,' ', K_CAPS, 0, 0, 0, 0, 0, 
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
     0, 0, 0, K_DEL, 0, K_NUM, K_SCROLL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

/* Use the keymap specified in the config file. */
static uint16 keymap = BCON_KBD_KEYMAP;

/* Keyboard input queue, which contains ascii values for keys that have been
 * input.  This is used on a bcon->read. */
static keyqueue_t input_queue = {
  .head = 0,
  .tail = 0,
  .count = 0
};

static uint16 kbd_state = 0;

static int enqueue( char c )
{
  if( input_queue.count == BCON_KBD_QUEUE_LEN ) { 
    /* BEEP HERE. */
    puts( "\nQueueFull\n" );
    return RET_FAILURE; 
  }

  input_queue.keys[input_queue.tail++] = c;
  input_queue.tail%= BCON_KBD_QUEUE_LEN;
  input_queue.count++;

  return RET_SUCCESS;
}

/* Dequeue can return a 0 because all of these keys will be 
 * non zero ascii characters.*/
static char dequeue()
{
  char retval;

  if( input_queue.count == 0 ) {
    /* BEEP HERE. */
    return 0;
  }

  retval = input_queue.keys[input_queue.head++];
  input_queue.head %= BCON_KBD_QUEUE_LEN;
  input_queue.count--;

  return retval;
}

int bcon_input_queue_head()
{
  char retval;

  if( input_queue.count == 0 ) { return RET_FAILURE; }

  retval = input_queue.keys[input_queue.head++];
  input_queue.head %= BCON_KBD_QUEUE_LEN;

  return retval;
}

/* Tell the keyboard to use our handler. */
uint32 bcon_kbd_init()
{
  bcon_kbd_echo_on();
  return bcon_kbd_arch_init();
}

void bcon_kbd_echo_on()
{
  kbd_state |= ECHO_STATE;
}

void bcon_kbd_echo_off()
{
  kbd_state &= ~ECHO_STATE;
}

/* State is either up (1) or down (0), the scancode is the scancode of the 
 * key from the keyboard. */
void bcon_kbd_handle( uint16 scancode, uint8 state )
{
  uint16 key;

  /* Handel shift and caps locked. */
  if( kbd_state&K_ST_SHIFT || kbd_state&K_ST_CAPS ) 
    key = keymaps[keymap][scancode+128];
  else 
    key = keymaps[keymap][scancode];

  /* ASCII key */
  if( key < 128 && key != 0 ) {

    /* Ignore ASCII key releases. */
    if( state == 1 ) { return; }

    /* Handle special cases. */
    switch( key ) {
    
    /* Map CarrageReturn to new line. */
    case '\r': key = '\n'; break;

    /* Remove character from input_queue queue. */
    case '\b': 
      if( dequeue() == 0 ) { return; } 
      break;
    }
              
    /* Don't add a '\b' to the input queue. */
    if( key != '\b' )
      if( enqueue( key ) == -1 ) { return; }

    /* Key echo on? */
    if( (kbd_state&ECHO_STATE) != 0 )
      bcon_putchar( key );

    return;
    
  /* Non-ASCII key */
  } else {
    switch( key ) {

    case K_CTL:
      if( state == 0 ) { kbd_state |= K_ST_CTL; }
      else { kbd_state &= ~K_ST_CTL; }
      break;

    case K_LMETA: /* Fall through. */
    case K_RMETA:
      if( state == 0 ) { kbd_state |= K_ST_META; }
      else { kbd_state &= ~K_ST_META; }
      break;

    case K_DEL:
      if( state == 0 ) { kbd_state |= K_ST_DEL; }
      else { kbd_state &= ~K_ST_DEL; }
      break;

    case K_LSHIFT: /* Fall through. */ 
    case K_RSHIFT:
      if( state == 0 ) { kbd_state |= K_ST_SHIFT; } 
      else { kbd_state &= ~K_ST_SHIFT; }
      break;

    case K_CAPS:
      if( state == 0 ) { kbd_state ^= K_ST_CAPS; }
      break;

    }

    if( state == 0 && (kbd_state&K_ST_CTL) && (kbd_state&K_ST_META) &&
        (kbd_state&K_ST_DEL) ) { arch_kbd_reboot(); }
  }

}
#endif /* CONFIG_KBD */
