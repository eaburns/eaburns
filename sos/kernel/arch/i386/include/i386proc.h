#ifndef _I386_PROC_H_
#define _I386_PROC_H_

#include <tss.h>
#include <memman.h>

typedef struct {

  tss_t* tss;

} proc_info_t;

#endif /* _I386_PROC_H_ */
