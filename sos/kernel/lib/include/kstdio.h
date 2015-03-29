#ifndef _KSTDIO_H_
#define _KSTDIO_H_

/* Prints a character to the screen .*/
void putchar( int c );

/* Prints a string to the screen. */
void puts( char *s );

/* kernel print format. */
void kprintf( char *format, ... );

#endif /* _KSTDIO_H_ */
