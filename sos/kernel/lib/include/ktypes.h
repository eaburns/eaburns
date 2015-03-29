#ifndef _KTYPES_H_
#define _KTYPES_H_

#ifndef __ASM__

/* The common posix types. */
#ifndef _NO_COMMON_
#include <common/types.h>
#endif

/* A few typedefs to get some lower level names for our data. */
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32; 


/* 32bit vs 64bit arch.  (only 32 is currently supported) */
#if 1
typedef unsigned long word_t;
#else
typedef unsigned long long word_t;
#endif /* 1 */

#endif /* !__ASM__ */

#endif /* _KTYPES_H_ */
