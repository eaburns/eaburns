#ifndef _CMOS_H_
#define _CMOS_H_

#include <ktypes.h>

/* Index numbers for CMOS. */
#define CMOS_FLOPPY_INDEX 0x10

uint32 cmos_get_data( uint32 index );

#endif /* _CMOS_H_ */
