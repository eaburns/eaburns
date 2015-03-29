#include <kstdlib.h>
#include <kstdio.h>
#include <cpu.h>
#include <init.h>
#include <console.h>
#include <interrupt.h>
#include <kstring.h>
#include <memutil.h>
#include <memman.h>
#include <sched.h>
#include <process.h>
#include <panic.h>

/* Max length of kernel arguments. */
#define ARG_LEN 50

#include <test_code.h>

/* Create a simple _cheating_ init process and add it to the scheduler. */
static void init_process()
{
  static process_t p1, p2, p3;

  create_process( &p1, &test_thread_1, &test_thread_1_end - &test_thread_1 );
  sched_add( &p1 );
  create_process( &p2, &test_thread_2, &test_thread_2_end - &test_thread_2 );
  sched_add( &p2 );
  create_process( &p3, &test_thread_3, &test_thread_3_end - &test_thread_3 );
  sched_add( &p3 );
}

extern uint32 get_free_frame( uint32* );

int kmain( uint32 _memsize, char *_arg )
{
  char args[ARG_LEN];

  memory_size = _memsize;

  /* Make sure to copy this string over to our kernel's memory so that we
   * do not stomp on it. */
  kstrcpy( args, _arg );

  /* A console should not be REQUIRED, however, it is... so there. */
  if( init_console() == RET_FAILURE ) { while(1) { halt(); } }

  INITIALIZE(init_cpu, "Unable to initializing cpu." );

  /* This will be removed some time... it's just to show that args 
   * still works. */
  kprintf( "kernel parameters: [%s]\n", args );

  call_init_functs();
  init_sched();

  /*  
   * This is for test only.
   */
  init_process();

  /* Enable interrupts begins the scheduler. */
  enable();
  while( 1 ) { halt(); }

  return 0;
}

/* This function is called after the init process is completed.
 * This will happen on a system halt, reboot, or anything else that will
 * cleanly shut down the operating system. */
void kend()
{
  enable(); while( 1 ) { halt(); }
}
