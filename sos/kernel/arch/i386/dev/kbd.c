#include <bus.h>
#include <interrupt.h>
#include <cpu.h>

#define KBD_CMD 0x64
#define KBD_DATA 0x60
#define KBD_READ_OUTPUT_PORT 0xD0
#define KBD_WRITE_OUTPUT_PORT 0xD1

/* Wait for input availability. */
#define KBD_INPUT_WAIT() \
do { \
  bus_space_read_8( I386_BUS_IO_T, KBD_CMD, 0, buff ); \
} while( (buff & 0x2) != 0 ) \

/* Flush the output, wait for it to be cleared. */
#define KBD_OUTPUT_FLUSH() \
do {  \
  bus_space_read_8( I386_BUS_IO_T, KBD_CMD, 0, buff );\
  if( (buff & 0x1) == 0 ) { break; }\
  bus_space_read_8( I386_BUS_IO_T, KBD_DATA, 0, buff ); \
} while( 1 ) \

void arch_kbd_reboot()
{
  uint32 buff; 

  /* Clear interrupts. */
  disable();

  KBD_INPUT_WAIT();
  KBD_OUTPUT_FLUSH();

  /* Raise the CPU reset line. */
  bus_space_write_8( I386_BUS_IO_T, KBD_CMD, 0, 0xFE );

  /* I suppose we shouldn't get here, so lets halt. */
  halt();
}
