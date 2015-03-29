#ifndef _KALLOC_H_
#define _KALLOC_H_

#include <ktypes.h>

/* Initialize kalloc.  Returns -1 on error or else it returns a 0. */
int kalloc_init( void* base, uint32 len );

/* Allocates kernel memory of the given size, and returns a pointer
 * to it, or NULL if there is not enough memory. */
void* kalloc( uint32 size );

/* Frees memory that was previously returned from a kalloc call. */
void kfree( void* slot );

/* Return the kalloc region base address. */
void* kalloc_get_base();

#endif /* _KALLOC_H_ */
