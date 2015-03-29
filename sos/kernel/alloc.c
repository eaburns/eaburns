/* This file contains functions that generalize kalloc and malloc to not 
 * be for a specific memory region.  These functions are -NOT- used for 
 * memory allocation of processes!!!!. */

#include <alloc.h>
#include <kstdlib.h>
#include <kstdio.h>
#include <retval.h>

/* A memory region is set up as follows:
 * __________________________________________
 * | freelist | usedlist | data_region...   |
 * |   head   |   head   |                  |
 * ------------------------------------------
 *
 * And the data region is made up of alloc_slot_t's
 * example data region:
 * ________________________________________
 * | next pointer | length |  data...     |
 * ----------------------------------------
 *
 * Initially, becides the free_list_head and the used_list_head
 * the whole memory region is a free block (an alloc_slot_t), therefor
 * the used_list_head is NULL, and the free_list_head points to the 
 * region base+2 (2 because 1 for each head pointer).
 */
int init_alloc( void *base, uint32 length )
{
  alloc_slot_t *elem;

  /* Make sure that there is enough space to setup a head and tail pointer. */
  if( length < (sizeof(void*)*2) ) { return RET_FAILURE; }

  /* Initialize used list to NULL. */
  A_USEDLIST(base) = NULL;

  /* Construct the initial free slot. */
  elem = (alloc_slot_t*)(((alloc_slot_t**)base)+2);
  elem->next = NULL;

  /* The actual length of the data portion of the initial free slot is 
   * the length of the memory region minus the free and used head pointers
   * and minus the alloc_slot_t structure imposed on the initial free slot
   * itself. */
  elem->len = length - (sizeof(alloc_slot_t*)*2) - sizeof(alloc_slot_t);

  /* Freelist points to the first elem. */
  A_FREELIST(base) = elem;

  return RET_SUCCESS;
}

void *alloc( void *base, uint32 size ) 
{
  alloc_slot_t *p, *j, *k;

  /* Make size be divisable by sizeof(word). */
  if( size%sizeof(void*) != 0 )
    size += sizeof(void*) - (size%sizeof(void*));

 /* Traverse the freelist untill either we get to the end with no
  * slots that are large enough, or untill we find a slot that is large
  * enough.  Also keep a pointer to the previous element. */
  for( p = A_FREELIST(base), j = NULL; 
       p != NULL && p->len < (size+sizeof(alloc_slot_t)); 
       j = p, p = p->next ) {
  } 

  /* No slot found large enough for desired size. */
  if( p == NULL ) {
    kprintf( "alloc: no slots large enough for allocation of %d bytes.\n", 
              size );
    return NULL;
  }

  /* Head element is large enough. */
  if( j == NULL ) {
    A_FREELIST(base) = A_FREELIST(base)->next;

  /* Non-head element is large enough. */
  } else {
    j->next = p->next;

  }

  /* If the slot is larger than desired, split it into a used block and a 
   * free block. */
  if( p->len >= size+sizeof(alloc_slot_t) ) {
    
    k = ((void*)p)+sizeof(alloc_slot_t)+size;
    k->next = A_FREELIST(base);
    k->len = p->len - size - sizeof(alloc_slot_t);
    A_FREELIST(base) = k;

    p->len = size;
  }

  /* Add allocated slot to used list. */
  p->next = A_USEDLIST(base);
  A_USEDLIST(base) = p;

  return ((void**)p)+(sizeof(alloc_slot_t)/sizeof(word_t));
}


int free( void *base, void *slot )
{
  alloc_slot_t *p, *j;

  /* A pointer to the slot we are freeing's alloc_slot_t. */
  alloc_slot_t *f = (alloc_slot_t*)(((void**)slot)-2);

  /* Used for merging free slots. */
  alloc_slot_t *before = NULL;     /* An adjacent free slot before ``slot.'' */
  alloc_slot_t *after = NULL;      /* An adjacent free slot after ``slot.'' */
  alloc_slot_t *after_prev = NULL; /* The previous element to after, in the 
                                    * free list. */
  
  /* When set, the free slot is added to the free list, this is used
   * for merging. */
  uint8 free_add = 1;


  /* Find the slot on the used list. */
  for( j = NULL, p = A_USEDLIST(base); p != NULL && p != f; j = p, p = p->next );

  /* The slot is not allocated. */
  if( p == NULL ) {
    kprintf( "Attempt to free unallocated memory.\n" );
    return RET_FAILURE; 
  }


  /* 
   * Remove from used list. 
   */

  /* Slot was the head elem. */
  if( j == NULL )
    A_USEDLIST(base) = A_USEDLIST(base)->next;

  /* Slot was not the head elem. */
  else 
    j->next = p->next;


  /*  
   * Add slot back to free list and merge if possible.
   */
  
  /* Check for adjacent slots before and after the free slot to merge with. */
  for( j = NULL, p = A_FREELIST(base); p != NULL; j = p, p = p->next ) {

    if( A_BLOCK_END(p) == ((void**)f) )
      before = p;
    else if( A_BLOCK_END(f) == ((void**)p) ) {
      after = p;
      after_prev = j;
    }
  }

  /* Easy case: There is a block before ours that is free, then just tack
   * our block onto the end of the before block. */
  if( before != NULL ) {
    before->len += sizeof(alloc_slot_t) + f->len;
    f = before; /* We are now the before block. */

    /* We just merge onto ``before'' so do not add the slot again to the
     * free list. */
    free_add = 0; 
  }

  /* ``Harder'' case: There is a slot after us.  Merge that slot onto our slot
   * and remove the after slot from the free list. */
  if( after != NULL ) {
    f->len += sizeof(alloc_slot_t) + after->len;

    /* Remove after from the list. */
    if( after_prev == NULL )
      A_FREELIST(base) = A_FREELIST(base)->next;
    else 
      after_prev->next = after->next;
  }

  /* If we are not already in the free list by now, then lets add our 
   * selves to it. */
  if( free_add == 1 ) {
    f->next = A_FREELIST(base);
    A_FREELIST(base) = f;
  }

  return RET_SUCCESS;
}

/* Print a alloc_slot_t list given the head of the list
 * and a pointer to itterate through the list with. */
#define A_PRINT_LIST( x, y ) \
do {\
  if( x == NULL ) \
    kprintf( "Empty\n" ); \
  else { \
    for( y = x; y != NULL; y = y->next ) { \
      kprintf( "0x%x: base=0x%x\tlen=%d (0x%x)\n", \
               ((void*)y)-sizeof(alloc_slot_t), y, y->len, y->len ); \
    } \
  } \
} while(0)

/* Print free blocks (for debugging). */
void alloc_print_free( void *base )
{
  alloc_slot_t *p;
  kprintf( "Alloc Free:\n" );
  A_PRINT_LIST( A_FREELIST(base), p );
}

/* Print the used blocks (for debugging). */
void alloc_print_used( void *base )
{
  alloc_slot_t *p;
  kprintf( "Alloc Used:\n" );
  A_PRINT_LIST( A_USEDLIST(base), p );
}
