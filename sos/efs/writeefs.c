/* 
 * This program writes a file to the efs device.
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#include "efs.h"
#include "block.h"
#include "superblock.h"
#include "inode.h"

/* Device to write to. */
static char *device = NULL;

/* Disk superblock. */
static struct superblock sb;

/* Name of the file to write. */
static char *filename = NULL;

/*
 * Function prototypes.
 */
/* This function reads a file from disk into BLOCK_SIZE buffers and writes
 * blocks to the efs. */
int read_write( const char *file, const char *dev ); 

#define print_help() \
do { \
  printf( "Useage:\n%s [--device <path>] <file name>\n", argv[0] );\
  return 0; \
} while( 0 )
  
int main( int argc, char **argv )
{
  int c, opt_index;

  /* Parse the arguments. */
#define ARG_DEVICE 0x2
  struct option long_opts[] = {
    {"device", 1, 0, ARG_DEVICE},
    { 0, 0, 0, 0 }
  };

  while( 1 /* CONST CONDITION. */ ) {
    c = getopt_long( argc, argv, "h", long_opts, &opt_index );
    if( c == -1 ) { break; }

    switch( c ) {
    case ARG_DEVICE:
      device = strdup(optarg); break;
    }
  }

  if( optind < argc )
    filename = argv[optind]; 
  else
    print_help();

  if( device == NULL )
    device = strdup("disk.img");

  if( validate_efs( device ) != 0 ) {
    printf( "A valid efs file system was not found on %s.\n", device );
    return 1;
  }
  
  /* Get the superblock. */
  bzero( &sb, SUPERBLOCK_LEN );
  get_superblock( &sb, device );

  read_write( filename, device ); 

  return 0;
}

int read_write( const char *file, const char *dev )
{
  char f_name[FILE_NAME_SIZE];
  char *file_name_nodir;
  uint8 block_buff[BLOCK_SIZE];
  uint32 block;
  struct inode inode;
  uint32 inode_num;
  int fd;
  int read_len;

  /* Get rid of directory crap. */
  file_name_nodir = rindex( file, '/' ); 
  if( file_name_nodir == NULL ) { file_name_nodir = (char*)file; }
  else { file_name_nodir++; }

  /* Truncate the name to 12 characters. */
  strncpy( f_name, file_name_nodir, FILE_NAME_SIZE-1 );

  if( new_file( dev, f_name ) == -1 ) {
    printf( "Error creating new file %s.\n", f_name );
    return -1;
  }

  if( (fd = open( file, O_RDONLY )) == -1 ) {
    perror( "Error reading from file" );
    return -1;
  }

  bzero( block_buff, BLOCK_SIZE );
  while( (read_len = read( fd, block_buff, BLOCK_SIZE )) != 0 ) {

    if( (block = falloc_block( dev, f_name )) == 0 ) {
      printf( "Error allocating a new block for file %s.\n", f_name );
      return -1;
    }

    if( write_block( block, block_buff, dev ) == -1 ) {
      printf( "Error writing block.\n" );
      return -1;
    }

    if( find_file( &inode_num, dev, f_name ) == -1 ) {
      printf( "Error getting inode number for file %s.", f_name );
      return -1;
    }

    if( get_inode( &inode, inode_num, dev ) == -1 ) {
      printf( "Error reading inode %d.\n", inode_num );
      return -1;
    }

    inode.size += read_len;

    if( set_inode( inode_num, &inode, dev ) == -1 ) {
      printf( "Error setting inode %d.\n", inode_num );
      return -1;
    }

    bzero( block_buff, BLOCK_SIZE );
  }

  return 0;
}
