#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>

#include "block.h"
#include "superblock.h"
#include "inode.h"

/* Default (number of 1024byte blocks on a 1.44M floppy). */
static int block_count = 1440;

/* Device to create file system on. */
static char *device = NULL;

/* Number of inodes. */
static int inode_count = 0;

static struct superblock *sb = NULL;


/* 
 * Function prototypes.
 */

/* Writes a new superblock to the disk. */
static void create_superblock();

/* Writes the inode blocks. */
static void create_inodeblocks();

/* Creates a root directory with nothing in it. */
static void create_rootdir();


#define print_help() \
do { \
  printf( "Useage:\n%s [-h | --help] [--blocks <num>] [--device <path>]\n", argv[0] );\
  return 0; \
} while( 0 )
  

int main( int argc, char **argv )
{
  int c, opt_index;

  /* Parse the arguments. */
#define ARG_BLOCKS 0x1
#define ARG_DEVICE 0x2
  struct option long_opts[] = {
    {"blocks", 1, 0, ARG_BLOCKS},
    {"device", 1, 0, ARG_DEVICE},
    {"help", 0, 0, 'h'},
    { 0, 0, 0, 0 }
  };

  while( 1 /* CONST CONDITION. */ ) {
    c = getopt_long( argc, argv, "h", long_opts, &opt_index );
    if( c == -1 ) { break; }

    switch( c ) {
    case ARG_BLOCKS:
      block_count = atoi(optarg); break;
    case ARG_DEVICE:
      device = strdup(optarg); break;
    case 'h':
      print_help();
    }
  }

  if( device == NULL )
    device = strdup("disk.img");
    //print_help();   
  
  create_superblock(); 
  create_inodeblocks();
  create_rootdir();

  if( device != NULL )
    free( device );

  return 0;   
}

#define inode_blocks 4
void create_superblock()
{
  int inodes_per_block = BLOCK_SIZE/sizeof(struct inode);
  uint8 block[BLOCK_SIZE];

  sb = new_superblock();

  bzero( block, BLOCK_SIZE );

  sb->inode_offset = 1; 
  sb->inode_count = inodes_per_block * inode_blocks;
  inode_count = sb->inode_count;
  sb->data_offset = 1+inode_blocks;
  sb->data_count = block_count - sb->data_offset;
  
  memcpy( block+BOOTCODE_LEN, sb, SUPERBLOCK_LEN ); 

  write_block( 0, block, device );

  free( sb );
}

void create_inodeblocks()
{
  int i;
  uint8 block[BLOCK_SIZE];

  bzero( block, BLOCK_SIZE );

  for( i = sb->inode_offset; i < sb->data_offset; i++ )
    write_block( i, block, device );
}

/* NOTE: */
/* A directory entry is 12 bytes of a name and 4 bytes of an inode index. */

void create_rootdir()
{
  struct inode inode;  
  uint8 block[BLOCK_SIZE];

  /* The director has only itself as an entry. */
  bzero( block, BLOCK_SIZE );
  strcpy( block, "." );
  block[12] = 0;      /* Root directory is inode 0. */

  /* Write the root directory block .*/
  write_block( sb->data_offset, block, device ); 

  bzero( &inode, INODE_LEN );
  inode.size = 16;    /* 16 byte size to start. */
  inode.refs = 1;     /* There is 1 reference to this inode. */
  inode.data0 = 0;    /* logical data block 0 is real data block 0. */

  /* Write root directory inode. */
  set_inode( 0, &inode, device );

  /* Mark the data block used. */
  if( mark_block_used( 0, device ) != 0 ) {
    printf( "Error marking sector 0 as used.\n" );
    exit( 1 );
  }

  /* No other info matters till I feel like implementing it. */
}
