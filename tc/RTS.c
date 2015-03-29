/* 
 * RTS.c This file contains implementation for the T run time system.
 * Ethan Burns -- Created Thu Mar 30 11:01:17 EST 2006
 */ 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

/* Generic arrayVMT */
extern volatile void *RTS_arrayVMT;

static void 
RTS_error(const char *msg, unsigned int line)
{
	fflush(stdout);
  fprintf(stderr, "ERROR: %s (line %u)\n", msg, line);
  exit(-1);
}

#define ARRAY_HEADER_LEN (sizeof(void*)*4) /* Bytes. */

/* Creates an array given the address of the dimension list. */
static void*
RTS_createArrayDimLoc(const unsigned int line, void *baseVMT,
						const unsigned int rank,
						unsigned int numDims,
						int *dims)
{
	int *len;
	int i;
	uintptr_t *base;

	if(rank == 0) { return NULL; }

	/* Get the length of this dimension. */
	len = dims + (numDims - 1);

	if(*len < 0)
		RTS_error("Negative array size", line);

	base = calloc(1, sizeof(void*) * (*len) + ARRAY_HEADER_LEN);
	
	/* Set the header information for the array. */
	base[0] = (uintptr_t) &RTS_arrayVMT; /* The array vmt. */
	base[1] = (uintptr_t) baseVMT;      /* The base type vmt. */
	base[2] = rank;              /* The array rank. */
	base[3] = *len;              /* The length of the array. */

	for(i = 0; i < *len; i++) {
		base[i+4]= (uintptr_t)RTS_createArrayDimLoc(line, baseVMT, rank-1, numDims-1, dims);
	}

	return base;
}

void*
RTS_createArray(const unsigned int line, void *baseVMT, const unsigned int rank,
								int numDims, ...)
{
	return RTS_createArrayDimLoc(line, baseVMT, rank, numDims, &numDims+1);
}

void 
RTS_checkNullReference(const unsigned int line, const void* addr)
{
  if(addr == NULL) {
    RTS_error("Null reference.", line);
  }
}

void
RTS_checkNarrowingConversion(const unsigned int line, void* dst, void **src)
{
  void *vmtp;

	RTS_checkNullReference(line, src);

  for(vmtp = *src; vmtp != NULL && vmtp != dst; vmtp = *(void**)vmtp);

  if(vmtp != dst) {
    RTS_error("Invalid cast", line);
  }
}

void 
RTS_checkMalloc(const unsigned int line, void* addr)
{
  if(addr == NULL) {
    RTS_error("Out of memory.", line);
	}
}

/* Reverses `num' arguments on the stack following `num'. */
void 
RTS_reverseArgumentsOnStack(unsigned int num)
{
  unsigned int *p, *q;
  unsigned int tmp;

  p = (&num)+1;
  q = (&num)+num;

  while(p < q) {
    tmp = *p;
    *p = *q;
    *q = tmp;

    p++, q--;
  }
}

void 
RTS_outputInteger(int i)
{
  printf("%d\n", i);
}

void 
RTS_checkForZeroDivide(unsigned int line, const int divisor)
{
  if(divisor == 0) {
    RTS_error("Divide by zero.", line);
	}
}
