#ifndef _MEMUTIL_H_
#define _MEMUTIL_H_

#include <ktypes.h>

/* Copies 'bytes' bytes of memory from src buffer to dest buffer. */
void memcpy( void *dest, const void *src, uint32 bytes );

/* Sets 'bytes' bytes of memory in the dest buffer to 'val.' */
void memset( void *dest, uint8 val, uint32 bytes );

/* Sets ``len'' bytes to zero starting at ``mem.'' */
#define bzero( x, y ) memset( x, 0, y )

#endif /* _MEMUTIL_H_ */
