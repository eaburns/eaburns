#ifndef _MEMMAN_H_
#define _MEMMAN_H_

#include <ktypes.h>

typedef struct {
  unsigned ns:1;    /* No swap bit. */
  unsigned res:7;   /* Reserved bits. */
  unsigned count:8; /* Reference count. */
} frame_t;

typedef struct {
  word_t *pd;     /* Page directory. */
  uint32 num_frames; /* Number of elements in the frames list below. */
  word_t *frames; /* List of frames allocated to our process, in order of
                   * lowest virt address to highest virt address in the
                   * pd/pt. */
  uint32 num_pts;
  word_t *pts;    /* Page table frame list. */
} addr_space_t;

/* The size in bytes of main memory (passed to the kernel by the boot loader) */
extern uint32 memory_size;

/* Number of frames in physical memory. */
extern uint32 frame_count;

/* Initializes the memory manager. */
int init_memman();

/* Marks frame `f' as a used frame with the given flags. 
 * RET_SUCCESS or RET_FAILURE is returned depending on if there is a 
 * failure or not. */
int memman_use_frame( uint32 f, uint32 flags );

/* Frees a frame in memory (by marking it unused) */
int memman_free_frame( uint32 f );

#define FRAME_NO_SWAP 1

#define FRAME_KERN (FRAME_NO_SWAP)
#define FRAME_USER 0

#define MEMMAN_ALLOC_USER_FRAME( e ) \
  ((word_t)memman_alloc_frame( &error, FRAME_USER ))

#define MEMMAN_ALLOC_KERN_FRAME( e ) \
  ((word_t)memman_alloc_frame( &error, FRAME_KERN ))

/* Allocates a frame from memory and returns the frames physical address.
 * ``error'' is set to 0 on success and to 1 on error. 
 * The second argument is the flags to give the allocated frame. */
void *memman_alloc_frame( uint32 *error, uint32 flags );

/* Allocates address space for a process of size ``size'' bytes. 
 * return corresponds to retval.h */
/*
   __________________________________________________________ 
   NOTE: This does not free frames on a failure, this is WEAK
   ---------------------------------------------------------- 
*/
int alloc_addr_space( addr_space_t *as, uint32 size );

/* Frees the given address space for a process.  Returns retval.h values. */
int free_addr_space( addr_space_t *as );
#endif /* _MEMMAN_H_ */
