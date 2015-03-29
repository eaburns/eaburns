#ifndef __KERNEL__
  #include <string.h>
  #include <stdlib.h>
  #include <stdio.h>
#else
  #include "memutil.h"
#endif /* __KERNEL__ */

#include "superblock.h"
#include "block.h"
#include <print.h>

struct superblock *new_superblock()
{
  struct superblock *retval; 

#ifndef __KERNEL__
    retval = (struct superblock*) malloc( sizeof(struct superblock) );
    if( retval == NULL ) {
      PRINT( "Not enough memory to allocate a superblock.\n" );
      return NULL;
    }

#else
    /* We need kalloc here. */
    PRINT( "This feature is not yet implemented.\n" );
    return NULL;
#endif /* __KERNEL__ */

  memset( retval, 0, sizeof(struct superblock) );

  retval->ident[0] = 0xFF;
  retval->ident[1] = 'E';
  retval->ident[2] = 'f';
  retval->ident[3] = 'S';

  return retval;
}

int get_superblock( struct superblock *sb, const char *dev )
{
  uint8 block[BLOCK_SIZE];

  if( read_block( 0, block, dev ) == -1 ) {
    PRINT( "Error reading superblock.\n" );
    return -1;
  }

  memcpy( sb, block+BOOTCODE_LEN, SUPERBLOCK_LEN );

  return 0;
}
