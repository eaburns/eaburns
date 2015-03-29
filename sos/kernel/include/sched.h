#ifndef _SCHED_H_
#define _SCHED_H_

#include <process.h>
#include <ktypes.h>

/* Maximum number of processes... this should be moved to .config some time. */
#ifndef SCHED_MAX_PROCS
#define SCHED_MAX_PROCS 4
#endif /* SCHED_MAX_PROCS */

/* Currently running pid. */
extern uint32 curproc;

/* Initializes the scheduler. */
int init_sched();

/* Add a process to the scheduler. */
int sched_add( process_t *p );

/* Context switches to the next process on the list. */
void schedule();

/* Kills a process with the given pid.  This function will free-up the pages
 * associated with the given process that it is freeing. */
void process_kill( uint32 pid );

/* Wrapper function for the text of a process. */
void sched_start_proc();

/* Performs the actual context switch, this is arch dependent. */
void context_switch( process_t *to, process_t *from );

/* Arch specific scheduler add process. */
void arch_sched_add( process_t *p );

#endif /* _SCHED_H_ */
