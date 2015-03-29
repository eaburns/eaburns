#ifndef _SCREENIO_H_
#define _SCREENIO_H_

#include <ktypes.h>

/* Functions declaired in this file are defined in the architecture
 * specific sections, and must be defined in order for certain functionality
 * of the kernel. */

/* Scrolls the screen buffer up to make an empty line at the bottom of 
 * the screen and homes the cursor on the new blank line.*/
void __inline bcon_scroll_screen_up();

/* Clears a line on the screen. */
void __inline bcon_clear_line( int y );

/* Writes a character to the screen at vid_x, vid_y with the 
 * attributes given. */
void __inline bcon_write_char( int c, int attributes );

/* Sets the cursor position. */
void __inline bcon_set_cursor_pos( uint32 x, uint32 y );

/* Returns the cursor position via the arguments. */
void __inline bcon_get_cursor_pos( uint32* x, uint32* y );

/* Returns terminal sizes set by the arch. */
uint32 __inline bcon_get_term_rows();
uint32 __inline bcon_get_term_cols();

/* Arch specific bcon keyboard init. */
uint32 bcon_kbd_arch_init();

#endif /* _SCREENIO_H_ */
