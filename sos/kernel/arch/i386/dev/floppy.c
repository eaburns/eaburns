#include <floppy.h>
#include <dma.h>
#include <init.h>
#include <bus.h>
#include <cmos.h>
#include <ktypes.h>
#include <kstdio.h>
#include <retval.h>

static uint32 iot = I386_BUS_IO_T;

/* Floppy controller status register. */
static uint32 io_status = 0x3F4; /* Read Only. */
/* Floppy controller digital output register. */
#if 0
static uint32 io_dout = 0x3F2; /* Read/Write. */
#endif
/* Floppy controller configuration controll register. (sets transfer rate). */
#if 0
static uint32 io_cfct = 0x3F7; /* Write Only. */
#endif 

/* Detected floppy devices. */
static floppy_drive fd0, fd1;

/* Data transfer rates for the cfct register. */
#define CFCT_500 0x0 /* 500 kbit/s */
#define CFCT_300 0x1 /* 300 kbit/s */
#define CFCT_250 0x2 /* 250 kbit/s */
#define CFCT_1M  0x3 /* 1 Mbit/s */

/* This table should help with figuring out which rate to use:
 * +------------+-----------+----------------+
 * | Drive type | Disk type | Transfer rate  |
 * +------------+-----------+----------------+
 * | 360 KB     | 360 KB    | 250 kbit/s     |
 * +------------+-----------+----------------+
 * | 720 KB     | 720 KB    | 250|300 Kbit/s |
 * +------------+-----------+----------------+
 * | 1.2 MB     | 360 KB    | 300 Kbit/s     |
 * |            +-----------+----------------+
 * |            | 1.2 MB    | 500 Kbit/s     |
 * +------------+-----------+----------------+
 * | 1.44 MB    | 720 MB    | 250 Kbit/s     |
 * |            +-----------+----------------+
 * |            | 1.44 MB   | 500 Kbit/s     |
 * +------------+-----------+----------------+ */


/* Floppy controller status bit flags. */
#define FDC_ACTIVE_A 0x1  /* Drive A is active if set. */
#define FDC_ACTIVE_B 0x2  /* Drive B is active if set. */
#define FDC_ACTIVE_C 0x4  /* Drive C is active if set. */
#define FDC_ACTIVE_D 0x8  /* Drive D is active if set. */
#define FDC_BUSY     0x10 /* FDC is bussy, don't write a command till clear. */
#define FDC_NDMA     0x20 /* Set if non-dma mode. */
#define FDC_DIO      0x40 /* Direction of IO, set fdc->cpu, clear cpu->fdc. */
#define FDC_MRQ      0x80 /* Set if the data register is not ready for a 
                             read or write. */

/* Floppy digital data register bits. (Low 2 bits are for disk select). */
#define FDC_DIO_RESET   0x04 /* Resets the controller. */
#define FDC_DIO_DMA_IRQ 0x08 /* Disable irq and dma. */
#define FC_DIO_MOTOR_A  0x10 /* When set, drive a's motor is on. */
#define FC_DIO_MOTOR_B  0x10 /* When set, drive b's motor is on. */
#define FC_DIO_MOTOR_C  0x10 /* When set, drive c's motor is on. */
#define FC_DIO_MOTOR_D  0x10 /* When set, drive d's motor is on. */

char *floppy_type[] = {"No", "360KB 5.25in", "1.2MB 5.25in", 
                       "720KB 3.5in", "1.44MB 3.5in", "2.88MB 3.5in" };
                        
/* Returns the status register's value. */
static __inline__ uint8 fdc_get_status()
{
  uint8 retval;

  bus_space_read_8( iot, io_status, 0, retval );
  return retval;
}

/* Enables the motor on the given drive, 0-3. */
#if 0
static void fdc_enable_motor( uint32 d )
{
  uint8 drive = (0xF&d)<<4;

  bus_space_write_8( iot, io_dout, 0, drive );
}
#endif

/* Sets static fd0 and fd1 variables. */
static void floppy_probe()
{
  uint32 cmos_data;

  cmos_data = cmos_get_data( CMOS_FLOPPY_INDEX );  

  fd0.type = (cmos_data>>4)&0xF;
  fd1.type = cmos_data&0xF;
}

/* Called durring device initialization. */
int floppy_init()
{
  ONLY_ONCE();

  floppy_probe();

  /* Print some information about which floppy devices were found. */
  if( fd0.type == 0 && fd1.type == 0 ) { 
    kprintf( "No floppy drives detected.\n" );
    return RET_PROBE_FAILURE;
  }
  kprintf("fd0: %s floppy drive found.\n", floppy_type[fd0.type] );
  if( fd1.type != 0 ) 
    kprintf("fd1: %s floppy drive found.\n", floppy_type[fd1.type] );

  return RET_SUCCESS;
}

init_info( "fd", floppy_init );
