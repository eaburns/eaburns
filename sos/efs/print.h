/* We only need to use this macro and print will work in kernel mode
 * and non-kernel mode. */
#include <stdio.h>

#ifdef __KERNEL__
#define PRINT(x...) kprintf( x )
#else
#define PRINT(x...) fprintf( stderr, x )
#endif
