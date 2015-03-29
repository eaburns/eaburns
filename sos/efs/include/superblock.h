#ifndef _SUPERBLOCK_H_
#define _SUPERBLOCK_H_

#include <ktypes.h>

/* The superblock of efs. */
struct superblock {

  /* 512 bytes are reserved for the boot sector. */
//  char reserved[512] __attribute__((packed));

  /* Ident for EFS ( 0xFF, 'E','f', 'S' ). */
  char ident[4] __attribute__((packed));

  /* Start block of the inode table. */
  uint32 inode_offset __attribute__((packed));

  /* Number of inodes. */
  uint32 inode_count __attribute__((packed));


  /* Start block of the data blocks */
  uint32 data_offset __attribute__((packed));

  /* Number of data blocks. */
  uint32 data_count __attribute__((packed));

};

#define SUPERBLOCK_LEN sizeof( struct superblock ) /* Bytes. */
#define BOOTCODE_LEN 512 /* Bytes. */

/* Creates a new superblock with all fields set to 0, 
 * except for the ident file which is filled out with the proper bytes. 
 * Upon error NULL is returned and the error message is printed to the
 * screen. */
struct superblock *new_superblock();

/* Gets a superblock from device ``dev.''  Returns a -1 on error. */
int get_superblock( struct superblock *sb, const char *dev );

#endif /* _SUPERBLOCK_H_ */
