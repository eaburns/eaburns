#ifndef _BLOCK_H_ 
#define _BLOCK_H_

#include <ktypes.h>
#include <inode.h>

#define BLOCK_SIZE 1024 /* bytes. */

/* Writes a block to the file system.  In kernel mode the last argument
 * is the device number, however, in non-kernel mode this is the name of
 * the device or file to write a block to.  -1 is returned on error and
 * the error message is printed to the screen. */
int write_block( uint32 lblock_num, const uint8 info[BLOCK_SIZE], const char *dev );

/* Reads a block from the disk into memory.  -1 is returned upon error. */
int read_block( uint32 lblock_num, uint8 buff[BLOCK_SIZE], const char *dev );

/* Marks a data block as used.  0 is returned on success, 1 is returned if the
 * block specified is already used, and -1 is returned on error. */
int mark_block_used( uint32 block_num, const char *dev );

/* Returns 0 if the block is not used, and -1 upon error. */
int check_block( uint32 block_num, const char *dev );

/* Finds a free block on the device ``dev.''  Returns 0 on error (no free block found). */
uint32 find_free_block( const char *dev );

/* Converts a data block number to a real block number.  Returns 0 on error. */ 
uint32 data_to_real( uint32 datablk, const char *dev );

/*
 * Blocks are counted from 0 up.  The superblock is in block 0, but is
 * started from a 512 byte offset that is reserved for the boot sector.
 */

/* Super block information. */
#define SUPERBLOCK 0
#define SUPERBLOCK_OFFSET 512

#endif /* _BLOCK_H_ */
