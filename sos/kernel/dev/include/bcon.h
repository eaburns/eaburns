#ifndef _BCON_H_
#define _BCON_H_

/* (B)asic console 
 * Ethan Burns <eaburns@cisunix.unh.edu>
 */
#include <ktypes.h>

/* Bright green terminal text color. */
/* #define TERM_ATTR 0x0A */

/* White on blue terminal text color. */
#define TERM_ATTR 0x1F

/* Grey on black terminal text color. */
/* #define TERM_ATTR 0x07 */

uint16 bcon_attrs;

uint32 bcon_read( uint8 *buff, uint32 len );
uint32 bcon_write( uint8 *buff, uint32 len );
int bcon_init();

void bcon_clear_screen();

void bcon_putchar( int c );


#endif /* _BCON_H_ */
