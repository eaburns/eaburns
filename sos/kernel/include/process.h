#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <ktypes.h>
#include <regs.h>
#include <memman.h>

typedef enum {
  PROC_NULL,  /* An un-used process in the proctab. */
  PROC_RUN,   /* A process that can be executed. */
  PROC_WAIT,  /* A process waiting for I/O. */
  PROC_ZOMBIE /* The process is a zombie(waiting for wait to be called on it)*/
} procstat_t;

typedef struct _process {
  void *arch_info;            /* Arch specific info. */
  procstat_t status;          /* The process's run status. */
  struct _process *parent;    /* The parent process of this process.  NULL
                                 for the init process. */
  addr_space_t *addr_space;   /* Address space for this process. */
  uint32 pid;                 /* PID number, the index into the proctab. */
  uint32 userid, groupid;     /* User/group that executed the process. */
  struct _process *next;      /* Used for adding processes to a wait queue. */
} process_t;

/* Creates a process.  p is the process struct to fill in, text is where the
 * text is located (it will be copied over into allocated frames in this 
 * function), t_len is the length of the text. */
int create_process( process_t *p, void* text, uint32 t_len );

/* Frees all memory that is allocated by arch specific ``create_process.''
 * This only includes kalloced memory, not memory from memman. */
void destroy_process( process_t *p );

#endif /* _PROCESS_H_ */
