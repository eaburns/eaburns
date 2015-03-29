#ifndef __KERNEL__
#include <stdio.h>
#include <string.h>
#else
#endif

#include "block.h"
#include "efs.h"
#include "superblock.h"
#include "inode.h"
#include "print.h"

/* Adds a file to a directory entry (namely root since we only support one directory
 * for now.  -1 is returned on error. */
static int add_file( const char *dev, const char *filename, int inode_num );

int validate_efs( const char *dev )
{
  struct superblock sb;

  /* Grab superblock from device */
  if( get_superblock( &sb, dev ) == -1 ) {
    PRINT( "Error getting superblock from device.\n" );
    return -1;
  }

  /* Check the ident bits. */ 
  if( sb.ident[0] != -1 &&  sb.ident[1] != 'E' &&
      sb.ident[2] != 'f' && sb.ident[3] != 'S' )
    return -1;
      
  return 0; 
}

int new_file( const char *dev, const char *filename )
{
#ifndef __KERNEL__
  int i;
  struct inode inode;
  struct superblock sb;

  if( get_superblock( &sb, dev ) == -1 ) {
    PRINT( "Error getting the superblock. \n" );
    return -1; 
  }

  /* If file exists truncate it. */
  if( find_file( &i, dev, filename ) != -1 ) {

    PRINT( "File exists... truncating.\n" );

    if( get_inode( &inode, i, dev ) == -1 ) {
      PRINT( "Error getting inode %d.\n", i );
      return -1;
    }  

    inode.size = 0; 
    
    if( set_inode( i, &inode, dev ) == -1 ) {
      PRINT( "Error setting inode %d.\n", i );
      return -1;
    }

    return 0;
  } 
  
  /* Check for a free inode. */
  for( i = 0; i < sb.inode_count; i++ ) {
    if( get_inode( &inode, i, dev ) == -1 ) {
      PRINT( "Unable to get inode number %d.\n", i );
      return -1;
    }

    /* In this case we have a free inode. */
    if( inode.refs == 0 ) { break; }
  }

  if( i >= sb.inode_count ) { 
    PRINT( "No free inodes on device.\n" );
    return -1;
  }

  /* Mark the inode taken. */
  inode.refs = 1;

  /* Write the new entry to the root directory (our only directory
   * for now). */
  if( add_file( dev, filename, i ) == -1 ) {
    PRINT( "Error adding directory entry to directory.\n" );
    return -1;
  }

  if( set_inode( i, &inode, dev ) == -1 ) {
    PRINT(" Error writing inode %d to device.\n", i );
    return -1;
  }

  return 0;

#else
  PRINT( "This feature is not yet supported.\n" );
#endif /* __KERNEL__ */
}

static int add_file( const char *dev, const char *filename, int inode_num )
{
  struct inode inode;
  uint32 last_block;
  uint32 data_block;
  uint8 block[BLOCK_SIZE];
  uint8 *blk_offset;

  /* Get the root inode. */
  if( get_inode( &inode, 0, dev ) == -1 ) {
    PRINT( "Error getting root directory inode.\n" );
    return -1;
  }

  /* Get the number of the last block of the file. */
  last_block = inode.size/BLOCK_SIZE; 

  if( (data_block = get_logical_block( &inode, last_block, dev )) == 0 ) {
    PRINT( "Error getting file logical block %d.\n", last_block );
    return -1;
  }

  if( read_block( data_block, block, dev ) == -1 ) {
    PRINT( "Error reading data block %d.\n", data_block );
    return -1;
  }

  /* Goto the correct offset in this block. */
  blk_offset = block+(inode.size%BLOCK_SIZE);

  /* Copy the name over. */
  strncpy( (char*)blk_offset, filename, FILE_NAME_SIZE-1 );

  /* Write this file's inode number. */
  blk_offset += FILE_NAME_SIZE;
  *((uint32*)blk_offset) = inode_num;

  /* Write the block back to the disk. */
  if( write_block( data_block , block, dev ) == -1 ) {
    PRINT( "Error writing the file block back to the disk.\n" );
    return -1;
  }
  /* The directory is now 16 bytes larger. */
  inode.size += 16;

  if( set_inode( 0, &inode, dev ) == -1 ) {
    PRINT( "Error writing root inode to disk.\n" );
    return -1;
  }

  return 0;     
}

int find_file( uint32 *inode, const char *dev, const char *filename )
{
  uint32 i, j;
  uint32 last_block;
  uint32 data_block;
  struct inode root_inode;
  uint8 block[BLOCK_SIZE];
  
  /* Get the root inode. */
  if( get_inode( &root_inode, 0, dev ) == -1 ) {
    PRINT( "Error getting root directory inode.\n" );
    return -1;
  }

  /* Get the number of the last block of the file. */
  last_block = root_inode.size/BLOCK_SIZE; 

  for( i = 0; i <= last_block; i++ ) {

    data_block = get_logical_block( &root_inode, i, dev ); 
    if( data_block == 0 ) {
      PRINT( "Error getting data block %d from inode.\n", i );
      return -1;
    } 

    if( read_block( data_block, block, dev ) == -1 ) {
      PRINT( "Error reading block %d.\n", data_block );
      return -1;
    }

    for( j = 0; j < BLOCK_SIZE; j += (FILE_NAME_SIZE+4) ) {

      /* File found. */
      if( strncmp( block+j, filename, FILE_NAME_SIZE ) == 0 ) {
        *inode = *(uint32*)(block+j+FILE_NAME_SIZE); 
        return 0;
      }
    }
    
  }

  /* File not found. */
  return -1;
}

uint32 falloc_block( const char *dev, const char *filename )
{
  uint32 inode, block;
  struct superblock sb;

  if( get_superblock( &sb, dev ) == -1 ) {
    PRINT( "Error getting superblock.\n" );
    return -1;
  }

  if( find_file( &inode, dev, filename ) == -1 ) {
    PRINT( "Error finding newly created file %s.\n", filename );
    return -1;
  }

  if( (block = find_free_block( dev )) == 0 ) {
    PRINT( "No free blocks on device %s.\n", dev );
    return -1;
  }

  if( mark_block_used( block, dev ) == -1 ) {
    PRINT( "Error marking block %d as used.\n", block );
    return -1;
  } 

  if( add_block_pointer( inode, block, dev ) == -1 ) {
    PRINT( "Error adding inode pointer to inode %d.\n", inode );
    return -1;
  }   

  return data_to_real( block, dev );
}
