#ifndef _BCON_KBD_H_
#define _BCON_KBD_H_

#include <config/bcon.h>
#include <ktypes.h>
#include <kstdlib.h>

/* Key queue. */
typedef struct {
  uint16 head;
  uint16 tail;
  uint16 count;
  char keys[BCON_KBD_QUEUE_LEN]; /* ASCII keys. */
} keyqueue_t;

/* Non ascii key. */
#define NA_KEY(x) (128+x)\

/* Standard Non-ASCII keys (Keep track of their states too!). */
#define K_ST_SHIFT  1<<0
#define K_ST_META   1<<1
#define K_LSHIFT    NA_KEY(0)
#define K_ST_LSHIFT K_ST_SHIFT
#define K_LMETA     NA_KEY(1)
#define K_ST_LMETA  K_ST_META
#define K_CTL       NA_KEY(2)
#define K_ST_CTL    1<<2
#define K_FN        NA_KEY(3)
#define K_ST_FN     1<<3
#define K_CAPS      NA_KEY(4)
#define K_ST_CAPS   1<<4
#define K_LEFT      NA_KEY(5)
#define K_ST_LEFT   1<<5
#define K_RIGHT     NA_KEY(6)
#define K_ST_RIGHT  1<<6
#define K_UP        NA_KEY(7)
#define K_ST_UP     1<<7
#define K_DOWN      NA_KEY(8)
#define K_ST_DOWN   1<<8
#define K_WIN       NA_KEY(9)
#define K_ST_WIN    1<<9
#define K_RSHIFT    NA_KEY(10)
#define K_ST_RSHIFT K_ST_SHIFT
#define K_RMETA     NA_KEY(11)
#define K_ST_RMETA  K_ST_META
#define K_NUM       NA_KEY(12)
#define K_ST_NUM    1<<10
#define K_SCROLL    NA_KEY(13)
#define K_ST_SCROLL 1<<11
#define K_DEL       NA_KEY(14)
#define K_ST_DEL    1<<12

#define ECHO_STATE  1<<15

uint32 bcon_kbd_init();

/* Handles key input from the keyboard given the key's scancode
 * and up/down state.
 * State is either up (1) or down (0), the scancode is the scancode of the 
 * key from the keyboard. */
void bcon_kbd_handle( uint16 scancode, uint8 state );

/* Turns on/off key echos. */
void bcon_kbd_echo_on();
void bcon_kbd_echo_off();

/* Returns the head element of the input queue, or -1 if it is empty. */
int bcon_input_queue_head();
#endif /* _BCON_KBD_H_ */
