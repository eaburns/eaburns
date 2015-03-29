#ifndef _EFS_H_
#define _EFS_H_

#include "inode.h"

#define FILE_NAME_SIZE 12

/* Checks if the file system is an efs device.  This is accomplished
 * by checking the ident bytes in the superblock.  A 0 is returned
 * if the device contains an efs file system. */
int validate_efs( const char *dev );

/* Makes a new empty file on the device ``dev.''  The return value 
 * is a 0 on success and a -1 on error.  If the file already exists
 * in the root directory, then it is meerly truncated to zero. */
int new_file( const char *dev, const char *filename );

/* Finds a file and returns it's inode number via the first arugment.  The function
 * returns -1 on error, or file not found. and 0 on success. */ 
int find_file( uint32 *inode, const char *dev, const char *filename );
               
/* Allocates a block for a file and adds the block to the inode of that file.  The
 * return value is the block that was allocated or a 0 on error. */
uint32 falloc_block( const char *dev, const char *filename );

#endif /* _EFS_H_ */
