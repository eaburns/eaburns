OBJS=inode.o superblock.o block.o efs.o
INCLUDE=-I./include -I../kernel/lib/include -I.

# USE THESE CFLAGS ONLY IF NOT BUILDING
# FOR KERNEL SUPPORT!!!!
CFLAGS=-g -Wall $(INCLUDE) -D_NO_COMMON_ -static

all: efs.a util

efs.a: $(OBJS)
	ar r efs.a $(OBJS)

util: efs.a mkefs.o writeefs.o
	$(CC) $(CFLAGS) mkefs.o efs.a -o mkefs
	$(CC) $(CFLAGS) writeefs.o efs.a -o writeefs

disk.img:
	dd if=/dev/zero of="disk.img" count=1474560 bs=1

clean:
	rm -f makefile.bak $(OBJS) efs.a mkefs mkefs.o writeefs writeefs.o
