#include <kalloc.h>
#include <alloc.h>
#include <kstdio.h>
#include <ktypes.h>
#include <kstdlib.h>

static void* kalloc_base = NULL;

#include <config/kern.h>
#ifdef KALLOC_ROBUST
static uint32 inited = 0;
#endif /* KALLOC_ROBUST */

int kalloc_init( void* base, uint32 len )
{
  kalloc_base = base;

#ifdef KALLOC_ROBUST
  inited = 1;
#endif /* KALLOC_ROBUST */

  kprintf( "kalloc: base: 0x%x, len: %d.\n", base, len );
  return init_alloc( base, len );
}

void* kalloc( uint32 size )
{
#ifdef KALLOC_ROBUST
  if( inited == 0 ) { 
    kprintf( "WARNING: kalloc used before it is initialized.\n" );
    return NULL; 
  }
#endif /* KALLOC_ROBUST */

  return alloc( kalloc_base, size );
}

void kfree( void* slot )
{
#ifdef KALLOC_ROBUST
  if( inited == 0 ) { return; }
#endif /* KALLOC_ROBUST */

  if( free( kalloc_base, slot ) == -1 )
    kprintf( "WARNING: Attempting to free unallocated memory in kfree.\n" ); 
}

void* kalloc_get_base() { return kalloc_base; }
