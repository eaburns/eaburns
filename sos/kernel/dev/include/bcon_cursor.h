#ifndef _BCON_CURSOR_H_
#define _BCON_CURSOR_H_

#include <ktypes.h>

/* Move the cursor by 1 position in the given direction. */
void __inline bcon_cursor_up();
void __inline bcon_cursor_down();
void __inline bcon_cursor_left();
void __inline bcon_cursor_right();

/* Put the cursor at the beginning of a line. */
void __inline bcon_cursor_home_line();

/* Returns 1 if the cursor is off the end of the screen, or 0 if not. */
uint8 __inline bcon_cursor_off_end();

#endif /* _BCON_CURSOR_H_ */
