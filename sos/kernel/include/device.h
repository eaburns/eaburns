#ifndef _DEVICE_H_
#define _DEVICE_H_

#include <kstdio.h>
#include <ktypes.h>
#include <fs.h>
#include <process.h>

/* Device major numbers. */
#define MAJOR_CHAR_UNNAMED 0
#define MAJOR_BLOCK_UNNAMED 0
#define MAJOR_CHAR_MEM 1
#define MAJOR_BLOCK_RAMDISK 1
#define MAJOR_CHAR_PTTY 2
#define MAJOR_BLOCK_FD 2
#define MAJOR_CHAR_PTTY_SLAVE 3
#define MAJOR_BLOCK_IDE 3
#define MAJOR_CHAR_CONS 4

typedef struct {
  void* dev_spec;         /* Device specific data. */
  char *name;             /* Device's name. */
  uint8 minor;            /* Devcie's minor number. */
  uint8 major;            /* Devcie's major number (is this needed?). */
  file_ops_t fops;        /* File operations. */
  process_t *wait_queue;  /* Procs waiting for I/O to complete on this dev. */

} device_t; 

/* Device read/write call back functions. */
typedef uint32 (*devrfun_t)( uint8* buff, uint32 len );
typedef uint32 (*devwfun_t)( uint8* buff, uint32 len );

#endif /* _DEVICE_H_ */
