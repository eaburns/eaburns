#include <paging.h>
#include <i386paging.h>
#include <memutil.h>
#include <kstdio.h>
#include <retval.h>
#include <panic.h>
#include <memman.h>

#define HALT_FINAL() \
  __asm__ __volatile__( "cli; hlt" )

/* To init paging, we identity map the first 4MB of memory to the kernel's 
 * page table.  Then we set bit 31 of cr0. */
int init_paging()
{
  uint32 i, j, pdes, error;
  void* addr = 0;
  word_t *p;

  kern_page_dir = (word_t)PD_BASE;
  /* This division will have MOD of 0.  If not, there is an awkward ammount
   * of memory (not 4K divisable) and we just ignore the leftover. */
  pdes = frame_count/PT_ENTRIES;    /* Number of required page directory 
                                       entires to map all physical frames. */

  /* Zero out the page table. */
  bzero( (void*)kern_page_dir, FRAME_LEN );

  /* Map all of physical memory to the kernel page table. */
  for( i = 0; i < pdes; i++ ) {
    p = (word_t*)MEMMAN_ALLOC_KERN_FRAME( &error );
    if( error == 1 ) { 
      kprintf( "Error allocating kernel frame for PT.\n" );
      return RET_FAILURE;
    }

    for( j = 0; j < PT_ENTRIES; j++ ) {
      p[j] = frame_to_pte_user( (word_t)addr );
      addr += FRAME_LEN;
    }

    ((word_t*)kern_page_dir)[i] = frame_to_pde_user( (word_t)p );
  }

  /* Load the page directory into cr3. */
  __asm__ __volatile( "movl %0, %%eax; \
                       movl %%eax, %%cr3" : : "r" (kern_page_dir) ); 

  /* Enable paging. */
  enable_paging();

  return RET_SUCCESS;
}

word_t frame_to_pte_user( word_t f ) 
{
  return f|PG_PRESENT|PG_USER|PG_READWRITE;
}

word_t frame_to_pde_user( word_t f )
{
  return f|PG_PRESENT|PG_USER|PG_READWRITE;
}

word_t frame_to_pte_kernel( word_t f )
{
  return f|PG_PRESENT|PG_READWRITE;
}

word_t frame_to_pde_kernel( word_t f )
{
  return f|PG_PRESENT|PG_READWRITE;
}
