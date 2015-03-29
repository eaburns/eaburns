#ifndef __KERNEL__
  #include <stdlib.h>
  #include <stdio.h>
  #include <string.h>
#else
  #include "memutil.h"
#endif /* __KERNEL__ */

#include "print.h"
#include "inode.h"
#include "block.h"
#include "superblock.h"

/* Number of blocks that can be pointed to by an indirect access block. */
#define INDIRECT_POINTERS (BLOCK_SIZE/sizeof(uint32))

struct inode *new_inode()
{
  struct inode *retval;

#ifndef __KERNEL__ 
    retval = malloc( sizeof(struct inode) ); 
    if( retval == NULL ) {
      PRINT( "Unable to allocate memory for a new inode.\n" );
      return NULL;
    }
#else
    /* use kalloc if we are in kernel mode. */
    PRINT( "This is not yet implemented.\n" );
    return NULL;
#endif /* __KERNEL__ */

  /* Set the fields to null. */
  memset( retval, 0, sizeof( struct inode ) );

  return retval;
}

int set_inode( int num, const struct inode *new, const char *dev )
{
  uint8 block[BLOCK_SIZE];
  uint32 block_num, inode_num;
  struct superblock sb;

  if( get_superblock( &sb, dev ) == -1 ) {
    PRINT( "Error getting the superblock. \n" );
    return -1; 
  }

  if( num > sb.inode_count ) { return -1; }

  /* Which inode on which block do we fix. */
  block_num = num / (BLOCK_SIZE/INODE_LEN) + sb.inode_offset;
  inode_num = num % (BLOCK_SIZE/INODE_LEN);

  /* Read the inode block into memory. */
  if( read_block( block_num, block, dev ) == -1 )
    exit( 1 );

  /* Copy the inode over. */
  memcpy( block+(inode_num*INODE_LEN), new, INODE_LEN );

  /* Store the block on disk. */
  if( write_block( block_num, block, dev ) == -1 )
    exit( 1 );
  
  return 0;
}

int get_inode( struct inode *retval, int num, const char *dev )
{
  uint8 block[BLOCK_SIZE];
  uint32 block_num, inode_num;
  struct superblock sb;

  if( get_superblock( &sb, dev ) == -1 ) {
    PRINT( "Error getting the superblock. \n" );
    return -1; 
  }

  if( num > sb.inode_count ) { return -1; }

  /* Which inode on which block do we fix. */
  block_num = num / (BLOCK_SIZE/INODE_LEN) + sb.inode_offset;
  inode_num = num % (BLOCK_SIZE/INODE_LEN);

  if( read_block( block_num, block, dev ) == -1 ) {
    PRINT( "Error reading block: %d.\n", block_num );
    return -1;
  }

  memcpy( retval, block+(inode_num*INODE_LEN), INODE_LEN );

  return 0;
}

uint32 get_logical_block( const struct inode *inode, uint32 lblock, const char *dev )
                       
{
  uint32 data_block;
  uint8 blk[BLOCK_SIZE];
  uint32 *indir_blk;
  uint32 indir_blk_num;
  uint32 d_indir_offset;
  uint32 *d_indir_blk;
  struct superblock sb;

  /* Direct data blocks. */
  if( lblock <= 4 ) {
    switch( lblock ) {
    case 0: data_block = inode->data0; break;
    case 1: data_block = inode->data1; break;
    case 2: data_block = inode->data2; break;
    case 3: data_block = inode->data3; break;
    case 4: data_block = inode->data4; break;
    }

  /* Indirect data blocks. */
  } else if( lblock > 4 && lblock <= INDIRECT_POINTERS ) {

    if( read_block( inode->indirect_data, blk, dev ) == -1 ) {
      PRINT( "Error reading indirect access block %d.\n", 
              inode->indirect_data );
      return 0;
    }

    /* Data block is found in a 4 byte offset in the indirect block. */
    indir_blk = (uint32*)blk;
    data_block = *(indir_blk+(lblock-5));

  /* Assume anything else is a double indirect data block. */
  } else {
    if( read_block( inode->double_indirect_data, blk, dev ) == -1 ) {
      PRINT( "Error reading double indirect access block %d.\n", 
              inode->double_indirect_data );
      return 0;
    }

    d_indir_offset = (lblock-INDIRECT_POINTERS)/INDIRECT_POINTERS;

    /* indirect block is found in a 4 byte offset in the indirect block. */
    d_indir_blk = (uint32*)blk;
    indir_blk_num = *(d_indir_blk+d_indir_offset);

    if( read_block( indir_blk_num, blk, dev ) == -1 ) {
      PRINT( "Error reading indirect access block %d.\n", indir_blk_num );
      return 0;
    }

    /* Data block is found in a 4 byte offset in the indirect block. */
    indir_blk = (uint32*)blk;
    data_block = *(indir_blk+(lblock-5));
  }

  if( get_superblock( &sb, dev ) == -1 ) {
    PRINT( "Error getting the superblock. \n" );
    return -1; 
  }

  return (data_to_real( data_block, dev ) == 0 ? 
                                             -1: 
               data_to_real( data_block, dev ));
}

int add_block_pointer( uint32 inode_num, uint32 block_num, const char *dev )
{
  uint8 block[BLOCK_SIZE];
  struct inode inode;
  struct superblock sb;
  uint32 *indir_blk;
  uint32 next_block;
  uint32 tmp;

  if( get_inode( &inode, inode_num, dev ) == -1 ) {
    PRINT( "Error getting inode %d.\n", inode_num );
    return -1;
  }

  /* Find the next logical block for the file. */
  next_block = inode.size/BLOCK_SIZE;
  
  /* Direct data blocks. */
  if( next_block <= 4 ) {

    PRINT( "Setting a direct data block pointer.\n" );
    switch( next_block ) {
    case 0: inode.data0 = block_num; break;
    case 1: inode.data1 = block_num; break;
    case 2: inode.data2 = block_num; break;
    case 3: inode.data3 = block_num; break;
    case 4: inode.data4 = block_num; break;
    }

    if( set_inode( inode_num, &inode, dev ) == -1 ) {
      PRINT( "Error saving inode %d to device.\n", inode_num );
      return -1;
    }

  /* Indirect data blocks. */
  } else if( next_block > 4 && next_block <= INDIRECT_POINTERS ) {

    PRINT( "Setting an indirect data block pointer.\n" );
    /* Check if we need to allocate a new indirect_data block. */
    if( inode.indirect_data == 0 ) {
      if( (tmp = find_free_block( dev )) == 0 ) {
        PRINT( "No space left on device to allocate an indirect data block.\n" );
        return -1;
      }

      if( mark_block_used( tmp, dev ) == -1 ) {
        PRINT( "Error marking block %d used.\n", tmp );
        return -1;
      }

      if( get_superblock( &sb, dev ) == -1 ) {
        PRINT( "Error getting the superblock. \n" );
        return -1; 
      }

      inode.indirect_data = tmp;
      if( set_inode( inode_num, &inode, dev ) == -1 ) {
        PRINT( "Error saving inode %d to device.\n", inode_num );
        return -1;
      }

      /* Zero out the new inode. */
      bzero( block, BLOCK_SIZE );
      if( write_block( data_to_real(inode.indirect_data, dev), block, dev ) == -1 ) {
        PRINT( "Error writing indirect data pointer block.\n" );
        return -1;
      }

      PRINT( "New indirect data block allocated with block number %d.\n", tmp );
    }

    /* Here we KNOW that the inode has an allocated indirect access data
     * pointer block. */
    if( read_block( data_to_real(inode.indirect_data, dev), block, dev ) == -1 ) {
      PRINT( "Error reading indirect data pointer block.\n" );
      return -1;
    }

    indir_blk = (uint32*)block;
    *(indir_blk+(next_block-5)) = block_num;
    PRINT( "Adding %d to indirect pointer block %d at offset %d.\n",
           block_num, data_to_real(inode.indirect_data, dev), (next_block-5) );

    if( write_block( data_to_real(inode.indirect_data, dev), block, dev ) == -1 ) {
      PRINT( "Error writing indirect data pointer block.\n" );
      return -1;
    }

  /* Assume anything else is a double indirect data block. */
  } else {
    PRINT( "Double indirect access data blocks are not yet implemented.\n" );
  }

  return 0;
}
