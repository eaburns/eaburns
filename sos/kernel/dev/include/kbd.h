#ifndef _KBD_H_
#define _KBD_H_

#include <ktypes.h>
#include <init.h>

/* Keyboard input handler (called upon interrupt). */
typedef void (*kbd_handler_t)( uint16, uint8 );

/* Attach a keyboard to it's I/O memory and a key handler. */
int kbd_attach( uint32 type, void *base, uint32 off, kbd_handler_t h );

/* Initializes the keyboard. */
int kbd_init();

/* Handles the keyboard interrupt. */
int kbd_interrupt(void);

/* Enable/Disable the keyboard (via hardware irqs).  The keyboard should be
 * disabled before reading from the key buffer as to avoid it modifying from
 * under you as you read. */
void kbd_enable();
void kbd_disable();

/* Arch specific reset via the keyboard controller. */
void arch_kbd_reboot();

#endif /* _KBD_H_ */
