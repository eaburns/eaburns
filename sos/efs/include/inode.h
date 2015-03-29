#ifndef _INODE_H_ 
#define _INODE_H_

#include <ktypes.h>

struct inode {

  /* Size of file. */
  uint32 size __attribute__((packed));

  /* Number of references to this inode. */
  uint8 refs __attribute__((packed));

  /* 7 dirrect reference data blocks pointers. */
  uint32 data0, data1, data2, data3, 
         data4 __attribute__((packed));
    
  /* Single and double indirect data block pointers. */
  uint32 indirect_data __attribute__((packed));
  uint32 double_indirect_data __attribute__((packed));

  /* File flags. */
  uint32 flags __attribute__((packed));

  /* Protection flags. */
  uint32 permission __attribute__((packed));

  /* Owner gid and uid. */
  uint32 uid, gid __attribute__((packed));

  /* Last access time. */
  uint32 access_time __attribute__((packed));

  /* Last modification time. */
  uint32 mod_time __attribute__((packed));

  /* Device information. */
  uint32 dev_info __attribute__((packed));
};

#define INODE_LEN sizeof( struct inode ) /* Bytes. */

/* Creates a new inode and returns it with all the fields filled out to a 
 * default value of zero.  NULL is returned upon error and an error message
 * is printed to the screen. */
struct inode *new_inode();

/* Sets the inode at index ``num,'' on device ``dev'' to a new value.  
 * -1 is returned on error. */
int set_inode( int num, const struct inode *new, const char *dev );

/* Gets the inode at index ``num'' of the device ``dev.''  Returns -1 on error. */
int get_inode( struct inode *retval, int num, const char *dev );

/* Gets a logical file block from an inode and returns the real disk logical block
 * number. */
uint32 get_logical_block( const struct inode *inode, uint32 lblock, const char *dev );

/* Adds the given block number to the next spot on the inode.  Return -1 on error. */
int add_block_pointer( uint32 inode, uint32 block_num, const char *dev );

#endif /* _INODE_H_ */
