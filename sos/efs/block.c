#ifndef __KERNEL__
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <fcntl.h>
  #include <unistd.h>
  #include <stdio.h>
#endif /* __KERNEL__ */

#include <block.h>
#include <inode.h>
#include <print.h>
#include <superblock.h>

/* Writes a block to the file system. */
int write_block( uint32 lblock_num, const uint8 info[BLOCK_SIZE], const char *dev )
{

#ifndef __KERNEL__
  ssize_t fd;
  int bytes;
  int offset;

  /* Open the device. */
  if( (fd = open( dev, O_WRONLY )) == -1 ) {
    perror( "Error opening device for writing" );
    return -1;
  }

  /* Lseek to the correct block position. */
  offset = BLOCK_SIZE * lblock_num;
  if( lseek( fd, offset, SEEK_SET ) != offset ) {
    perror( "Lseek error" );
    return -1;
  }
  
  /* Write the block. */
  if( (bytes = write( fd, info, BLOCK_SIZE )) != BLOCK_SIZE ) {
    perror( "Error writing block to disk" );
    return -1;
  }

  /* Guess what this line does... */
  close( fd );

  return 0;

#else
  PRINT( "This is not implemented.\n" );
  return -1;
#endif

}

int read_block( uint32 lblock_num, uint8 buff[BLOCK_SIZE], const char *dev )
{

#ifndef __KERNEL__
  ssize_t fd;
  int bytes;
  int offset;

  /* Open the device. */
  if( (fd = open( dev, O_RDONLY )) == -1 ) {
    perror( "Error opening device for reading" );
    return -1;
  }

  /* Lseek to the correct block position. */
  offset = BLOCK_SIZE * lblock_num;
  if( lseek( fd, offset, SEEK_SET ) != offset ) {
    perror( "Lseek error" );
    return -1;
  }
  
  /* Read the block. */
  if( (bytes = read( fd, buff, BLOCK_SIZE )) != BLOCK_SIZE ) {
    perror( "Error reading block to disk" );
    return -1;
  }

  /* Guess what this line does... */
  close( fd );

  return 0;

#else
  PRINT( "This is not implemented.\n" );
  return -1;
#endif

}

int mark_block_used( uint32 block_num, const char *dev )
{
  int byte_num, bit_num;
  uint32 mask;
  uint8 block[BLOCK_SIZE];
  uint8 *sect_map;

  /* Find which bit we want to set of which block .*/
  byte_num = block_num / 8;
  bit_num = 7 - (block_num % 8);
  mask = 1<<bit_num;
  
  /* Grab the superblock. */
  if( read_block( 0, block, dev ) == -1 ) {
    PRINT( "Error reading superblock.\n" );
    return -1;
  }

  sect_map = (block+SUPERBLOCK_LEN+BOOTCODE_LEN);

  /* Check if the block is taken. */
  if( ((*(sect_map+byte_num)) & mask ) != 0 ) { return 1; }

  /* Set our bit. */
  *(sect_map+byte_num) |= mask;
  
  if( write_block( 0, block, dev ) == -1 ) {
    PRINT( "Error writing superblock.\n" );
    return -1;
  }

  return 0;
}

int check_block( uint32 block_num, const char *dev )
{
  int byte_num, bit_num;
  uint32 mask;
  uint8 block[BLOCK_SIZE];
  uint8 *sect_map;

  /* Find which bit we want to set of which block .*/
  byte_num = block_num / 8;
  bit_num = 7 - (block_num % 8);
  mask = 1<<bit_num;
  
  /* Grab the superblock. */
  if( read_block( 0, block, dev ) == -1 ) {
    PRINT( "Error reading superblock.\n" );
    return -1;
  }

  sect_map = (block+SUPERBLOCK_LEN+BOOTCODE_LEN);

  /* Check if the block is taken. */
  if( ((*(sect_map+byte_num)) & mask ) != 0 ) { return 1; }

  return 0;
}

uint32 find_free_block( const char *dev )
{
  int i;
  struct superblock sb;

  if( get_superblock( &sb, dev ) == -1 ) {
    PRINT( "Error getting the superblock. \n" );
    return -1; 
  }

  for( i = 0; i < sb.data_count; i++ ) {
    if( check_block( i, dev ) == 0 )
      return i;
  }

  return 0;
}

uint32 data_to_real( uint32 datablk, const char *dev )
{
  struct superblock sb;

  if( get_superblock( &sb, dev ) == -1 ) {
    PRINT( "Error getting the superblock. \n" );
    return -1; 
  }

  return datablk + sb.data_offset;
}
