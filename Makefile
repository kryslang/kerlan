


run: kernel
	cat $<


kasm.o: kernel.asm
	nasm -f elf32 $^ -o $@


kc.o: kernel.c
	gcc -m32 -c $^ -o $@

kernel: link.ld kasm.o kc.o
	ld -m elf_i386 -T $^ -o $@

clean:
	rm *.o


