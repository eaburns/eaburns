#ifndef _PAGING_H_
#define _PAGING_H_

#include <ktypes.h>
#include <config/kern.h>
#include <memspace.h>

/* There is no choice but a 4k page/frame size for now.  Another arch that uses
 * a new page size will need to use the .config and ifdefs to change this. */
#if 1
  #define FRAME_LEN  0x1000 
  #define PT_ENTRIES (FRAME_LEN/sizeof(word_t))
  #define PD_ENTRIES (FRAME_LEN/sizeof(word_t))
#endif

/* Offset into a page directory for the end of kernel address space (1GB). */
#define KERN_PD_OFFSET (MEMSPACE_USER_MEM/(FRAME_LEN*PT_ENTRIES))

#ifndef __ASM__

/* Physical address of the kernel's page directory. */
extern word_t kern_page_dir;

/* Maps a frame to the given virtual base address in the kernel's page table. */
void kern_map_frame( void* base_addr, word_t frame );

/* Adds a page table to the kernel's page directory at the given index.   If
 * the kernel does not have a present page table entry for the physical
 * address of the page table it is adding, a frame is allocated and this
 * page table is setup, so the kernel can manipulate the page table that
 * is is adding. */
void kern_add_pt( uint32 pd_index, void* pt );

/* Given a frame, this function sets permission bits for a valid user, 
 * read/write, present frame. */
word_t page_table_entry( word_t e );

/* Arch specific:
 * Takes a frame base address as the argument. 
 * Return value is a pte or pde setup correctly with user or kenrel read/write 
 * privlages. */
word_t frame_to_pte_user( word_t f );
word_t frame_to_pde_user( word_t f );
word_t frame_to_pte_kernel( word_t f );
word_t frame_to_pde_kernel( word_t f );

#endif /* !__ASM__ */

#endif /* _PAGING_H_ */
