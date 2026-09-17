#include "../libc/kprint.h"
#include "../cpu/isr.h"
#include "../libc/string.h"
#include "kernel.h"




void kmain(void){
	isr_install();
	irq_install();
	s8 *str = "my first kernel test, yaayyy! I will test this so i can see how does the next line work \nalso i will test manual next line";
	// volatile char *videoptr = (char*)0xb8000; // video memory position
	// unsigned int i = 0;
	// unsigned int j = 0;


	//clear screen:
	// while(j<80*25*2){
	// 	videoptr[j] = ' ';
	// 	videoptr[j+1] = 0x07; // gray on black
	// 	j = j + 2;
	// }
	// j = 0;
	kclear();

	//print message:
	// while(str[j]!='\0'){
	// 	videoptr[i] = str[j];
	// 	videoptr[i+1] = 0x07; // gray on black
	// 	j++;
	// 	i = i + 2;
 //
	// }

	kprint_at(str, 0, 5, 0x0f);
	return;
}


void user_input(char str[]){
	kprint("you typed: ");
	kprint(str);
	kprint("\n");
}

