#ifndef _FLOPPY_H_
#define _FLOPPY_H_

#include <ktypes.h>

typedef struct {

  /* Floppy type number:
   * 0: none
   * 1: 360KB
   * 2: 1.2MB
   * 3: 720KB
   * 4: 1.44MB
   * 5: 2.88MB */
  uint8  type;

} floppy_drive;

#endif /*_FLOPPY_H_ */
