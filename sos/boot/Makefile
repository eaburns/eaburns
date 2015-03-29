# Comment out this next line if you DO NOT want a boot prompt. 
# NOTE: if this line is commented the loader will load the file
#       ``kernel'' only.
NOPROMPT=-D__NOPROMPT__
#NOPROMPT=

###############################################################################
# DO NOT MODIFY BELOW HERE
###############################################################################
CC=gcc
CFLAGS=-Wall $(NOPROMPT)

all: boot1 boot2

boot1: boot1.o
	ld boot1.o -o boot1 -dN --oformat=binary -Ttext 0x7C00

boot2: boot2.o
	ld boot2.o -o boot2 -dN --oformat=binary -Ttext 0x9000

boot1.o: boot1.S
	$(CC) $(CFLAGS) -c boot1.S -o boot1.o

boot2.o: boot2.S
	$(CC) $(CFLAGS) -c boot2.S -o boot2.o

clean:
	rm -f boot1.o boot1 boot2.o boot2
