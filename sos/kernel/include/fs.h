#ifndef _FS_H_
#define _FS_H_

#include <ktypes.h>
#include <common/types.h>

typedef struct {
  char *name;
} file_t;

/* File operations struct holds file operations for a device. */
typedef struct {
  int (*lseek)(file_t *file, off_t offset, int whence);
  int (*read)(file_t *file, void* buff, size_t count);
  int (*write)(file_t *file, void* buff, size_t count);
/* int (*readdir)(file_t *file, struct dirent *dirp, unsigned int count); */
/* int (*select)(?); */
/* int (*ioctl)(file_t *file, int request, ...?) */
  int (*mmap)(file_t *file, void* start, size_t length, int prot, int flags,
              off_t offset);
  int (*open)(file_t *file);
} file_ops_t;

#endif /* _FS_H_ */
