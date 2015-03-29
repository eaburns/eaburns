#ifndef _DMA_H_
#define _DMA_H_

#include <ktypes.h>

/* These can be ORed together for the DMA mode byte. */
#define DMA_WRITE_TRANSFER 0x04 /* Read and write can not be ORed. */
#define DMA_READ_TRANSFER  0x08

#define DMA_SINGLE_MODE 0x40

/* This function is for programming a channel on the DMA controller. */
void dma_program_8( uint32 channel, uint32 addr, uint32 count, uint32 mode );

#endif /* _DMA_H_ */
