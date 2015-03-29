#include <init.h>
#include <kstdlib.h>
#include <kstdio.h>
#include <kalloc.h>
#include <kstring.h>

void call_init_functs()
{
  initdata_t *p;
  int ret;

  p = &__init_start;
  while( p < &__init_end ) {
    ret = (*p->init)();

    kprintf( "%s initialization ", p->name );
    if( ret == RET_SUCCESS )
      kprintf( "success.\n" );
    else if( ret == RET_PROBE_FAILURE )
      kprintf( "probe failed.\n" );
    else
      kprintf( "failure.\n" );

    p++;
  }
}
