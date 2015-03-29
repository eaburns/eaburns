#ifndef _INIT_H_
#define _INIT_H_

#include <ktypes.h>
#include <retval.h>

/* Preforms critical initialization. */
#define INITIALIZE( func, error ) \
do { \
  if( func() == RET_FAILURE ) { panic( error ); } \
} while( 0 )

/* Calls all functions pointed to in the .initfunct section. */
void call_init_functs();

/* initfunct_type defines an initialization function. 
 * this is used by ``dymanic'' initialization.  Argument is not used yet, 
 * however it might be some time. */
typedef int(*initfunct_t)();

typedef struct {
  char *name;       /* duh! */
  initfunct_t init; /* Device's init function. */
} initdata_t;

/* These globals contain the bounds of the .initfunct section. */
extern initdata_t __init_start, __init_end;

#define init_info( name, init_fun ) \
  static initdata_t __initfunct__##init_fun __attribute__((__used__)) __attribute__ ((__section__ (".initdata"))) = { name, init_fun } 

/* Make sure this function is called only once. */
#define ONLY_ONCE() \
static uint8 __called = 0;\
do {\
  if( __called == 1 ) { return RET_INITED; } \
  __called = 1; \
} while( 0 )\


#endif /* _INIT_H_ */
