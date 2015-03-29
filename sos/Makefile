DISKSIZE=1474560
MAKEFLAGS+=--no-print-directory

# Directory of EFS utils for linux (BSD?)
EFS_UTIL=efs

all: util disks
	@echo
	@echo "If bochs is installed run: make test"
	@echo "to test SOS in bochs"

util:
	cd $(EFS_UTIL); $(MAKE) util ; cd ../..

disks: fd.img hd.img
	cd boot; $(MAKE); cd ..
	cd kernel; $(MAKE); cd ..
	dd if=boot/boot1 of="fd.img" conv=notrunc
	dd if=boot/boot2 of="fd.img" conv=notrunc seek=2874
	$(EFS_UTIL)/writeefs --device ./fd.img kernel/kernel
	dd if=boot/boot1 of="hd.img" conv=notrunc
	dd if=boot/boot2 of="hd.img" conv=notrunc seek=2874
	$(EFS_UTIL)/writeefs --device ./hd.img kernel/kernel

fd.img: util
	dd if=/dev/zero of="fd.img" count=2880 bs=512
	$(EFS_UTIL)/mkefs --device ./fd.img

hd.img: fd.img
	dd if=/dev/zero of=hd.img bs=512 count=20480
	$(EFS_UTIL)/mkefs --device ./hd.img
	
backup:
	sh backup.sh

test:
	$(MAKE) disks
	echo cont | bochs -q

depend:
	cd kernel; $(MAKE) depend; cd ..
  
clean:
	cd boot ; $(MAKE) clean ; cd ..
	cd kernel ; $(MAKE) clean ; cd ..
