#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include <device.h>
#include <init.h>

typedef struct {
  char *name;   
  devrfun_t read;
  devrfun_t write;
  int (*init)(void); 
  uint32 priority;
} console;

console *cur_con;

/* Initialize the console. */
int init_console(void);

/* Returns a pointer to the console with the given name, or NULL. */
console *get_console( char *name );

#endif /* _CONSOLE_H_ */
