#ifndef _MEMSPACE_H_
#define _MEMSPACE_H_

/* The kernel memory is all virtual memory up to 1gb of memory (0x40000000). 
 * After (and inclusive of) the 1gb marker is reserved for user process 
 * memory. */

/* User space starts at 1gb into memory. */
#define MEMSPACE_USER_MEM 0x40000000

/* The end of kernel memory. */
#define MEMSPACE_KERN_END (MEMSPACE_USER_MEM-1)

#endif /* _MEMSPACE_H_ */
