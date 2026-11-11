# C_SOURCES = $(wildcard libc/*.c kernel/*.c cpu/*.c drivers/*.c)
# HEADERS = $(wildcard libc/*.h kernel/*.h cpu/*.h drivers/*.h)
# # Nice syntax for file extension replacement
# OBJ = ${C_SOURCES:.c=.o cpu/interrupt.o}
#
#
#
# CC = /usr/local/i386elfgcc/bin/i386-elf-gcc
# GDB = /usr/local/i386elfgcc/bin/i386-elf-gdb
# CFLAGS = -g -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs \
# 		 -Wall -Wextra -Werror
#
# all: kernel-001
# 	echo "done"
#
# run: kernel-001
# 	qemu-system-i386 -kernel $^
#
# #isr.o: isr.c isr.h interrupt.asm
# #	${CC} ${CFLAGS} -ffreestanding -c $< -o $@
# # kasm.o: kernel.asm
# # 	nasm -f elf $^ -o $@
#
#
# #kc.o: kernel.c
# #	${CC} ${CFLAGS} -ffreestanding -c $^ -o $@
#
# kernel-001: kernel/kernel-start.o ${OBJ}
# 	i386-elf-ld -T link.ld -o $@ $^
#
# debug: kernel-001
# 	qemu-system-i386 -s -kernel kernel-001 &
# 	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel-001"
#
# %.o: %.c ${HEADERS}
# 	${CC} ${CFLAGS} -ffreestanding -c $< -o $@
#
# %.o: %.asm
# 	nasm $< -f elf -o $@
#
# %.bin: %.asm
# 	nasm $< -f bin -o $@
#
# clean:drivers/*.o
# 	rm libc/*.o kernel/*.o cpu/*.o drivers/*.o
#
# remove-all: clean
# 	rm -f kernel-001
#
#




C_SOURCES = $(wildcard libc/*.c kernel/*.c cpu/*.c drivers/*.c)
HEADERS = $(wildcard libc/*.h kernel/*.h cpu/*.h drivers/*.h)
OBJ = ${C_SOURCES:.c=.o cpu/interrupt.o}

CC = /usr/local/i386elfgcc/bin/i386-elf-gcc
GDB = /usr/local/i386elfgcc/bin/i386-elf-gdb
CFLAGS = -g -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs \
		 -Wall -Wextra -Werror

all: os-image.iso hdd.img
	@echo "Sestavení dokončeno!"

# Spuštění kernelu s CD (GRUB + Kernel) a připojeným pevným diskem (hdd.img)
run: os-image.iso hdd.img
	qemu-system-i386 -drive format=raw,file=hdd.img,media=disk,index=0 -drive format=raw,file=os-image.iso,media=cdrom,index=2 -boot d -d int,cpu_reset -D qemu.log -no-reboot

kernel-001: kernel/kernel-start.o ${OBJ}
	i386-elf-ld -T link.ld -o $@ $^

# Zabalení GRUBu a kernelu do bootovatelného ISO obrazu
os-image.iso: kernel-001
	mkdir -p iso_root/boot/grub
	cp kernel-001 iso_root/boot/
	@echo 'menuentry "My OS" { multiboot /boot/kernel-001; boot }' > iso_root/boot/grub/grub.cfg
	grub-mkrescue -o os-image.iso iso_root

# Vytvoření čistého 32MB FAT16 virtuálního pevného disku
hdd.img:
	dd if=/dev/zero of=hdd.img bs=1M count=32
	#mkfs.vfat -F 16 hdd.img

debug: os-image.iso hdd.img
	qemu-system-i386 -s -S -drive format=raw,file=hdd.img,media=disk,index=0 -drive format=raw,file=os-image.iso,media=cdrom,index=2 -boot d -d int,cpu_reset -D qemu.log -no-reboot &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel-001"

%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

clean:
	rm -f libc/*.o kernel/*.o cpu/*.o drivers/*.o kernel-001 os-image.iso hdd.img qemu.log
	rm -rf iso_root
