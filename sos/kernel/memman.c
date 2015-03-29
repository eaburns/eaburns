#include <memman.h>
#include <ktypes.h>
#include <memory.h> /* ARCH_FRAME_SIZE */
#include <kstdio.h>
#include <memutil.h>
#include <kalloc.h>
#include <retval.h>
#include <kstdlib.h>
#include <memspace.h>

/* Dynamically allocated bitmap of used memory frames. */
uint32 memory_size;           /* In kilo bytes. */
uint32 frame_count = 0;
static frame_t *frame_map;
static uint32 inited = 0;

/* If the the argument is not 0, there is an error (memory is full). */
uint32 get_free_frame( uint32 *error )
{
  uint32 i;
  *error = 0;
  if( inited != 1 ) { *error = 1; return 0; }

  for( i = 0; i < frame_count; i++ ) {
    if( frame_map[i].count == 0 )
      return i;
  }

  *error = 1;
  return 0;
}

int init_memman()
{
  uint32 frame_elems = 0;

  frame_count = (memory_size*1024) / FRAME_LEN;
  frame_elems = (frame_count/32)+1;
  kprintf( "mem: memory_size: %d KB.\n", memory_size );
  kprintf( "mem: frame_count: %d.\n", frame_count );

  if( (memory_size*1024) % ARCH_FRAME_SIZE != 0 )
    kprintf( "WARNING: Memory size is not divisable by frame size.\n" );

  /* This never will be freed; you NEVER want to stop the memory manager. */
  frame_map = (frame_t*)kalloc(frame_elems*sizeof(frame_t));

  if( frame_map == NULL ) {
    kprintf( "memman: unable to allocate frame_map.\n" );
    return RET_FAILURE;
  }

  memset( frame_map, 0, (frame_elems*sizeof(uint32)) );

  /* Reserve the first 4MB for kernel code, this is kinda lame. */
  memset( frame_map, ~0, (0x400000/FRAME_LEN)/8 );

  inited = 1;
  kprintf( "mem initialization success.\n" );
  return RET_SUCCESS;
}

int memman_use_frame( uint32 f, uint32 flags )
{
  frame_map[f].count++;
  frame_map[f].ns = flags & FRAME_NO_SWAP;
  return RET_SUCCESS;
}

/* Allocates a frame from main memory. */
void *memman_alloc_frame( uint32 *error, uint32 flags )
{
  uint32 frame_num;   
  *error = 0; /* just incase somthing screws up in get_free_frame, this starts
                 off as 0... If you want it done right, do it yourself. */

  frame_num = get_free_frame( error );

  /* Only mark the frame as used if there was no error. */
  if( *error != 1 ) {
    memman_use_frame( frame_num, flags ); 
    return (void*)(frame_num*ARCH_FRAME_SIZE);
  }

  return 0;
}

int memman_free_frame( uint32 f )
{
  if( frame_map[f].count != 0 )
    frame_map[f].count -= 1;
  return RET_SUCCESS;
}

int alloc_addr_space( addr_space_t *as, uint32 size )
{
  uint32 i, j, proc_frames, error;

  as->num_frames = (size/FRAME_LEN)+1;
  proc_frames = as->num_frames; /* Temp var. */
  as->frames = kalloc( as->num_frames*sizeof(word_t) );
  as->num_pts = ((as->num_frames)/PT_ENTRIES)+1;
  as->pts = kalloc( as->num_pts*sizeof(word_t) );

  as->pd = (void*)MEMMAN_ALLOC_USER_FRAME( &error );
  if( error == 1 ) {
    kprintf( "Error allocating user frame for PD.\n" );
    kfree( as->frames );
    kfree( as->pts );
    return RET_FAILURE;
  }

  bzero( as->pd, FRAME_LEN );

  for( i = 0; i < as->num_pts; i++ ) {

    /* Allocate a page table. */
    as->pts[i] = MEMMAN_ALLOC_USER_FRAME( &error );
    if( error == 1 ) { 
      kprintf( "Error allocating user frame for PT.\n" );
      memman_free_frame( (word_t)as->pd/FRAME_LEN );
      kfree( as->frames );
      for( i -= 1; i >= 0; i-- )
        memman_free_frame( (word_t)as->pts[i]/FRAME_LEN );
      kfree( as->pts );
      return RET_FAILURE;
    }

    /* For each needed frame in this table. */ 
    for( j = 0; j < PT_ENTRIES && proc_frames != 0; j++ ) {
      /* Not sure why I need to add 1 to this alloced value... but I do. */
      as->frames[j] = MEMMAN_ALLOC_USER_FRAME( &error )+1;
      if( error == 1 ) {
        kprintf( "Error allocating user frame.\n" );
        memman_free_frame( (word_t)as->pd/FRAME_LEN );
        for( j -= 1; j >= 0; j-- )
          memman_free_frame( (word_t)as->frames[j]/FRAME_LEN );
        kfree( as->frames );
        for( i -= 1; i >= 0; i-- )
          memman_free_frame( (word_t)as->pts[i]/FRAME_LEN );
        kfree( as->pts );
        return RET_FAILURE;
      }
      ((word_t*)as->pts[i])[j] = frame_to_pte_user(as->frames[j]);
      proc_frames--;
    }

    /* The first 1gb is going to be maped to the kernel. */
    as->pd[i+KERN_PD_OFFSET] = frame_to_pde_user(as->pts[i]);
  }

  /* Map the kernel page tables to the new page directory. */
  for( i = 0; i < KERN_PD_OFFSET; i++ ) {
    as->pd[i] = ((word_t*)kern_page_dir)[i];
  }

  return RET_SUCCESS;
}

int free_addr_space( addr_space_t *as )
{
  uint32 i;

  for( i = 0; i < as->num_frames; i++ )
    memman_free_frame( (word_t)as->frames[i]/FRAME_LEN );
  for( i = 0; i < as->num_pts; i++ )
    memman_free_frame( (word_t)as->pts[i]/FRAME_LEN );
  memman_free_frame( (word_t)as->pd/FRAME_LEN );

  kfree( as->frames );
  kfree( as->pts );

  return RET_SUCCESS;
}
