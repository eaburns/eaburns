#include <paging.h>
#include <process.h>
#include <sched.h>    /* sched_start_proc */
#include <ktypes.h>
#include <kalloc.h>
#include <tss.h>
#include <i386paging.h>
#include <i386proc.h>
#include <memman.h>
#include <retval.h>
#include <descriptor.h>
#include <kstdio.h>
#include <memutil.h>
#include <memspace.h>

#define DATA_LEN 0x10000
int create_process( process_t *p, void* text, uint32 t_len )
{
  uint32 i, copy_left;
  addr_space_t as;

  if( alloc_addr_space( &as, t_len+DATA_LEN) == RET_FAILURE ) {
    kprintf( "Unable to allocate process address space." );
    return RET_FAILURE;
  }

  /* Copy over the address space to the process struct. */
  p->addr_space = kalloc( sizeof(addr_space_t) );
  memcpy( p->addr_space, &as, sizeof(addr_space_t) );
  
  /* Copy program over to new address space. */
  copy_left = t_len;
  for( i = 0; i < as.num_frames; i++ ) {
    memcpy( ((void*)as.frames[i]), text, 
            copy_left>FRAME_LEN? FRAME_LEN: copy_left );
    copy_left -= FRAME_LEN;
  }

  /* Allocate a tss_t for the process. */
  p->arch_info = kalloc( sizeof(proc_info_t) );
  ((proc_info_t*)p->arch_info)->tss = kalloc( sizeof(tss_t) );

  /* DO NOT FORGET TO ALLOCATE A KERNEL STACK. */
  ((proc_info_t*)p->arch_info)->tss->esp0 = MEMSPACE_USER_MEM+t_len+DATA_LEN;
  ((proc_info_t*)p->arch_info)->tss->ss0 = KDATA_SELECTOR;

  ((proc_info_t*)p->arch_info)->tss->esp = MEMSPACE_USER_MEM+t_len+(DATA_LEN/2);
  ((proc_info_t*)p->arch_info)->tss->ss = UDATA_SELECTOR|3;

  ((proc_info_t*)p->arch_info)->tss->es = UDATA_SELECTOR|3;
  ((proc_info_t*)p->arch_info)->tss->ds = UDATA_SELECTOR|3;
  ((proc_info_t*)p->arch_info)->tss->fs = UDATA_SELECTOR|3;
  ((proc_info_t*)p->arch_info)->tss->gs = UDATA_SELECTOR|3;

  ((proc_info_t*)p->arch_info)->tss->cs = UCODE_SELECTOR|3;

  ((proc_info_t*)p->arch_info)->tss->eip = (uint32)sched_start_proc;

  ((proc_info_t*)p->arch_info)->tss->eflags = 0x200;
  ((proc_info_t*)p->arch_info)->tss->cr3 = (uint32)as.pd;

  return RET_SUCCESS;
}

/* This destroy function must free all kalloced memory from create_process. */
void destroy_process( process_t *p )
{
  kfree( ((proc_info_t*)p->arch_info)->tss );
  kfree( p->arch_info );
  kfree( p->addr_space );
}
