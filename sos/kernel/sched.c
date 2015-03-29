#include <sched.h>
#include <process.h>
#include <ktypes.h>
#include <kstdio.h>
#include <memutil.h>
#include <memspace.h>
#include <memman.h>
#include <kstdlib.h>
#include <timer.h>
#include <kernel.h>
#include <retval.h>
#include <interrupt.h>
#include <cpu.h>

process_t proctab[SCHED_MAX_PROCS];
uint32 curproc = 0;
static time_event_t sched_event;

int init_sched()
{
  uint32 i;

  for( i = 1; i < SCHED_MAX_PROCS; i++ )
    proctab[i].status = PROC_NULL;

  /* Setup the scheduler timer events. */
  time_event_set( &sched_event, hertz/10, TIME_EVENT_REPEAT, schedule, NULL );
  time_event_add( &sched_event );
  
  return RET_SUCCESS;
}

int sched_add( process_t *p )
{
  uint32 i;

  for( i = 1; i < SCHED_MAX_PROCS; i++ ) {
    if( proctab[i].status == PROC_NULL ) {
      memcpy( proctab+i, p, sizeof(process_t) );
      proctab[i].status = PROC_RUN;
      proctab[i].pid = i;
      arch_sched_add( proctab+i );
      return RET_SUCCESS;
    }
  }

  return RET_FAILURE;
}

/* Simple round-robbin for now, this should be made somthing better when
 * I get all the bugs out of the context switch. */
void schedule( void* a )
{
  uint32 lastproc = curproc;
  uint32 waiting_flag = 0;

  do {
    curproc = (curproc+1)%SCHED_MAX_PROCS;
    if( proctab[curproc].status == PROC_WAIT ) { waiting_flag = 1; }
  } while( (proctab[curproc].status != PROC_RUN || curproc == 0) && 
           lastproc != curproc );

  if( proctab[1].status == PROC_NULL ) {
    /* Init process terminated. */
  }
  /* This should be changed to ONLY check if proctab[1].status == PROC_NULL
   * since we only really should care about the init process.  For now
   * we check if all processes are null because there is no fork call :(
   *
   * Once we have fork and are checking proctab[1].status for NULLness,
   * we still need to check this case, If there are no processes that
   * aren't waiting or null, then we need to enable();halt(); so that
   * we can wait for a process to wake up. 
   *
   * ALSO: since we will probably want to implement waiting processes BEFORE
   * we make an actual fork call we can just use a flag to mark if there are
   * any sleeping processes, if there are, enable();halt(); if not (and since
   * init process will not yet be important) then we can kend(); */
  if( proctab[curproc].status != PROC_RUN ) {
    if( waiting_flag == 1 ) { enable(); halt(); return; }
    time_event_del( &sched_event );
    kprintf( "\n\nInitial process complete: halting.\n" );
    kend();
  }

  if( lastproc == curproc ) { return; }

  context_switch( proctab+curproc, proctab+lastproc );
}

void process_kill( uint32 pid )
{
  kprintf( "\nprocess %d terminating.\n", pid );
  free_addr_space( proctab[pid].addr_space );
  proctab[pid].status = PROC_NULL; 
  destroy_process( proctab+pid );
  schedule( NULL );
}

/* NOTE:
 * This should be re-written in asm and copied into user space then executed
 * from in user space so that the kernel pages do not need to be user 
 * accessable. */
void sched_start_proc()
{
  ((void(*)())MEMSPACE_USER_MEM)(); 
  __asm__ __volatile__( "movl $1, %eax; int $0x80" );
                        
}
