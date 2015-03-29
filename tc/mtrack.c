/*
 * mtrack.c
 * Implements functions for tracking allocated memory.
 * Ethan Burns -- Created Sun Feb 12 22:05:57 EST 2006
 */
#include <stdio.h>
#include <stdlib.h>
#include "mtrack.h"

static struct memassoc *mdefault = NULL;

/*
 * Initializes the default memory association. 
 */
static void
init_default()
{
  mdefault = make_memassoc();
}

/*
 * Returns a new memory association. 
 */
struct memassoc *
make_memassoc()
{
  struct memassoc *p;
  p = malloc(sizeof(struct memassoc));

  if (p == NULL) {
    fprintf(stderr, "malloc failed in function "
            "make_memassoc: Out of memory.\n");
    exit(1);
  }

  p->base = NULL;
  p->size = 0;
  p->next = NULL;

  return p;
}

/*
 * Allocates `bytes' bytes of memory, and returns a pointer to the
 * beginning of the memory.  A pointer to the allocated memory is kept in
 * the default memory association. 
 */
void *
t_mallocd(size_t bytes)
{
  if (mdefault == NULL)
    init_default();
  return t_malloc(mdefault, bytes);
}

/*
 * Allocates `bytes' bytes of memory, and returns a pointer to the
 * beginning of the memory.  A pointer to the allocated memory is added to 
 * the given memory association. 
 */
void *
t_malloc(struct memassoc *a, size_t bytes)
{
  struct memassoc *p;

  if (a->base == NULL) {
    /*
     * Head node was not used yet. 
     */

    a->size = bytes;
    a->base = malloc(bytes);

    if (a->base == NULL) {
      fprintf(stderr,
              "malloc failed in function t_malloc: Out of memory.\n");
      exit(1);
    }

    return a->base;
  }

  /*
   * Get next unused node. 
   */
  for (p = a; p->next != NULL; p = p->next);
  p->next = make_memassoc();
  p->next->size = bytes;
  p->next->base = malloc(bytes);

  if (p->next->base == NULL) {
    fprintf(stderr,
            "malloc failed in function t_malloc: Out of memory.\n");
    exit(1);
  }

  return p->next->base;
}

/*
 * Frees the memory unit with the given base address from the default
 * memory assoication. 
 */
void
t_freed(void *base)
{
  if (mdefault == NULL)
    init_default();
  t_free(mdefault, base);
}

/*
 * Frees the memory unit with the given base address from the given
 * memory assoication. 
 */
void
t_free(struct memassoc *a, void *base)
{
  struct memassoc *p,
  *j = NULL;

  for (p = a; p != NULL && p->base != base; j = p, p = p->next);

  if (p->base == base && j != NULL) {
    j->next = p->next;
    free(p->base);
    free(p);
  } else {
    fprintf(stderr,
            "Internal Error: attempt to free non-allocated memory!\n");
    exit(-1);
  }
}

/*
 * Frees all the memory units in the default memory association. 
 */
void
t_free_alld()
{
  if (mdefault == NULL)
    return;
  t_free_all(mdefault);
  mdefault = NULL;
}

/*
 * Frees all the memory units in the given memory association. 
 */
void
t_free_all(struct memassoc *a)
{
  struct memassoc *p,
  *j;

  p = a;
  while (p != NULL) {
    j = p;
    p = p->next;
    free(j->base);
    free(j);
  }
}
