C_SOURCES = $(wildcard libc/*.c kernel/*.c)
HEADERS = $(wildcard libc/*.h kernel/*.h)
# Nice syntax for file extension replacement
OBJ = ${C_SOURCES:.c=.o}



CC = /usr/local/i386elfgcc/bin/i386-elf-gcc
GDB = /usr/local/i386elfgcc/bin/i386-elf-gdb
CFLAGS = -g -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs \
		 -Wall -Wextra -Werror

all: kernel-001
	echo "done"

run: kernel-001
	qemu-system-i386 -kernel $^


# kasm.o: kernel.asm
# 	nasm -f elf $^ -o $@


#kc.o: kernel.c
#	${CC} ${CFLAGS} -ffreestanding -c $^ -o $@

kernel-001: kernel/kernel-start.o ${OBJ}
	i386-elf-ld -T link.ld -o $@ $^

debug: kernel-001
	qemu-system-i386 -s -kernel kernel-001 &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel-001"

%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

clean:
	rm libc/*.o kernel/*.o

remove-all: clean
	rm -f kernel-001


