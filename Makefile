
CC = /usr/local/i386elfgcc/bin/i386-elf-gcc
GDB = /usr/local/i386elfgcc/bin/i386-elf-gdb
CFLAGS = '-g'


run: kernel-001
	qemu-system-i386 -kernel $^


kasm.o: kernel.asm
	nasm -f elf $^ -o $@


kc.o: kernel.c
	${CC} ${CFLAGS} -ffreestanding -c $^ -o $@

kernel-001: kasm.o kc.o
	i386-elf-ld -T link.ld -o $@ $^

debug: kernel-001
	qemu-system-i386 -s -kernel kernel-001 &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel-001"


clean:
	rm *.o

remove-all:
	rm *.o kernel-001


