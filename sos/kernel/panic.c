#include <panic.h>
#include <cpu.h>
#include <kstdio.h>
#include <interrupt.h>

void panic( char *msg )
{
  disable();

  puts( "KERNEL PANIC: " );
  puts( msg );

  halt();  
} 
