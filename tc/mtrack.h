/*
 * mtrack.h
 * Definitions for memory allocation tracker.
 * Ethan Burns -- Created Sun Feb 12 21:59:07 EST 2006
 */
#ifndef _MTRACK_H_
#define _MTRACK_H_

/*
 * struct memassoc A list of associated memory allocation units. 
 */
struct memassoc {
  void *base;
  size_t size;
  struct memassoc *next;
};

/*
 * Returns a new memory association. 
 */
struct memassoc *make_memassoc();

/*
 * Allocates `bytes' bytes of memory, and returns a pointer to the
 * beginning of the memory.  A pointer to the allocated memory is kept in
 * the default memory association. 
 */
void *t_mallocd(size_t bytes);

/*
 * Allocates `bytes' bytes of memory, and returns a pointer to the
 * beginning of the memory.  A pointer to the allocated memory is added to 
 * the given memory association. 
 */
void *t_malloc(struct memassoc *a, size_t bytes);

/*
 * Frees the memory unit with the given base address from the default
 * memory assoication. 
 */
void t_freed(void *base);

/*
 * Frees the memory unit with the given base address from the given
 * memory assoication. 
 */
void t_free(struct memassoc *a, void *base);

/*
 * Frees all the memory units in the default memory association. 
 */
void t_free_alld();

/*
 * Frees all the memory units in the given memory association. 
 */
void t_free_all(struct memassoc *a);

#endif                          /* !_MTRACK_H_ */
