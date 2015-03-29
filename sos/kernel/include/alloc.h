#ifndef _ALLOC_H_
#define _ALLOC_H_

#include <ktypes.h>

/* The functions in this file are declarations of the general contiguous memory
 * allocation functions used for kalloc and malloc and the like.  These 
 * functions should work for any given list of free and allocated slots for
 * any set-up memory region. */

/* This struct is the beginning 8 (or more for 64+ bit archs) bits of a region
 * of allocatable memory used to from an allocated and a free list. */
typedef struct {
  /* A pointer to the next hole .*/
  void *next __attribute__((packed)); 

  /* The length of this hole. */
  uint32 len __attribute__((packed)); 
} alloc_slot_t;

/* This macro returns the last byte of the allocated region given an 
 * alloc_slot_t.
 * NOTE: this macro depends on the fact that all blocks allocated fall on a
 * one word boundry. */
#define A_BLOCK_END(x) ((void*)x)+sizeof(alloc_slot_t)+(x->len)

/* Given the base address of a memory region as the argument, these macros
 * evaluate to the freelist head and usedlist head respectivly. */
#define A_FREELIST(x) (*((alloc_slot_t**)x))
#define A_USEDLIST(x) (*((alloc_slot_t**)x+1))

/* Allocates a ``size'' byte region of memory from the given base region
 * and returns an address to the allocated section. */
void *alloc( void *base, uint32 size );

/* Frees an allocated slot of memory from the region specified by base.  
 * -1 is returned on error. */
int free( void *base, void *slot );

/* Sets up a memory region base as an allocatable region given the base address
 * and the length of the retion.  -1 is returned if there is a problem
 * initializing the given region. */
int init_alloc( void *base, uint32 length /* in bytes. */ );

/* Print free blocks (for debugging). */
void alloc_print_free( void *base );

/* Print the used blocks (for debugging). */
void alloc_print_used( void *base );

#endif /* _ALLOC_H_ */
