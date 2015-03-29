#include <memutil.h>
#include <ktypes.h>
#include <kstdio.h>

#include <config/kern.h>
#ifndef ARCH_I386

/* Copys memory. */
void memcpy( void *dest, const void *src, uint32 bytes ) 
{
  int i;

  for( i = 0; i < bytes; i++ )
    *((uint8*)dest+i) = *((uint8*)src+i);
}

/* Sets memory to value passed by second argument. */
void memset( void *dest, uint8 val, uint32 bytes )
{
  int i;

  for( i = 0; i < bytes; i++ )
    *((uint8*)dest+i) = val;
}

#endif /* ARCH_I386 */
