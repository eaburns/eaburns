ARCH=i386

###############################################################################
# No need to edit below here
#

CC=gcc
LD=ld
PWD=$(shell pwd)
INCLUDE=-I. -I$(PWD)/include
CFLAGS=-fno-builtin -Wall $(INCLUDE) -D__KERNEL__ -DARCH=$(ARCH) \
-fno-stack-protector -ansi
#CFLAGS=-fno-builtin -Wall $(INCLUDE) -D__KERNEL__ -DARCH=$(ARCH)
LDFLAGS=-T $(PWD)/arch/$(ARCH)/kernel.lds
MAKEFLAGS+= --no-print-directory --include-dir=$(PWD)
obj-y=$(start-obj)
obj-y+=$(PWD)/kmain.o
obj-y+=$(PWD)/irq.o
obj-y+=$(PWD)/interrupt.o
obj-y+=$(PWD)/panic.o
obj-y+=$(PWD)/init.o
obj-y+=$(PWD)/console.o
obj-y+=$(PWD)/alloc.o
obj-y+=$(PWD)/timer.o
obj-y+=$(PWD)/sched.o
obj-y+=$(PWD)/memman.o
obj-y+=$(PWD)/paging.o

obj-y+=$(PWD)/test_code.o

-include .config
-include arch/$(ARCH)/Makefile
-include lib/Makefile
-include dev/Makefile

###############################################################################
# Allow for linux style pretty output.
define cc_c_to_o
@echo CC $<
@$(CC) $(CFLAGS) -c $< -o $@
endef

%.o: %.c .config
	$(cc_c_to_o)

%.o: %.S .config
	$(cc_c_to_o)

###############################################################################
# Rules to build
all: config kernel

kernel: $(obj-y)
	rm -f kernel
	@echo LD $@
	@$(LD) $(LDFLAGS) $(obj-y) -o kernel -dN --oformat=binary


depend: config
	touch make.dep
	makedepend -fmake.dep $(INCLUDE) *.c lib/*.c arch/$(ARCH)/*.c

.PHONY: config

config:
	sh config.sh

clean:
	rm -f make.dep*; touch make.dep
	rm -f $(obj-y) kernel makefile.bak
	rm -f include/config/*.h

-include make.dep
