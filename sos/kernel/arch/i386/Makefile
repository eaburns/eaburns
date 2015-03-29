i386_DIR=$(PWD)/arch/i386
INCLUDE+=-I$(i386_DIR)/include
start-obj=$(i386_DIR)/start.o
obj-y+=$(i386_DIR)/io.o
obj-y+=$(i386_DIR)/cpu.o
obj-y+=$(i386_DIR)/trap.o
obj-y+=$(i386_DIR)/descriptor.o
obj-y+=$(i386_DIR)/8259_pic.o
obj-y+=$(i386_DIR)/8253_timer.o
obj-y+=$(i386_DIR)/irq.o
obj-y+=$(i386_DIR)/paging.o
obj-y+=$(i386_DIR)/paging_asm.o
obj-y+=$(i386_DIR)/cmos.o
obj-y+=$(i386_DIR)/process.o
obj-y+=$(i386_DIR)/sched.o
obj-$(ARCH_I386)+=$(i386_DIR)/memutil.o

-include $(i386_DIR)/dev/Makefile
