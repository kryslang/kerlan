#include "../libc/kprint.h"
#include "../cpu/isr.h"
#include "../libc/string.h"
#include "kernel.h"
#include "../drivers/disk.h"
#include "../libc/mem.h"



static u16 test[512];


static void disk_w_text(char *source){
	u32 lba = 100;
	//char *source = "hello there, this is a prfect test <>:::\0";
	u8 i;
	for(i = 0; i<(strlen(source)+1)/2+1; i++){
		test[i] = source[i*2];
		test[i] = test[i]+(source[i*2+1]<<8);
	}

	ata_write(lba, 1, test);
}

static void disk_test_r(){
	u32 lba = 100;
	ata_read(lba, 1, test);
}


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
	//kclear();




	//print message:
	// while(str[j]!='\0'){
	// 	videoptr[i] = str[j];
	// 	videoptr[i+1] = 0x07; // gray on black
	// 	j++;
	// 	i = i + 2;
 //
	// }

	kprint_at(str, 0, 5, 0x0f);
	while(1){
		//__asm__ __volatile__("");
	}

}


void user_input(char str[]){
	char first[20][256];
	u32 i = 0;
	u32 ni = 0;
	u8 n = 0;
	u8 st = 0;
	memset(first, 0, 20*256);
	while(str[i] != '\0'){

		if(!((str[i] == ' ' && (st==0)) || str[i] == '\0') && i<255){
			if(str[i] == '"'){
				st = st ^ 1;
				i++;
				continue;
			}
			first[n][ni] = str[i];
			i++;
			ni++;
		} else{
			first[n][ni]= '\0';
			i++;
			ni = 0;
			// while(!(str[i] == ' ' || str[i] == '\0') && i<255){
			// 	first[n][ni] = str[i];
			// 	i++;
			// 	ni++;
			// }

			n++;
		}
	}
	st = 0;
	first[n+1][0] = '\0';

	if(strcmp(first[0], "CLEAR") == 0){
		kclear();
	} else if(strcmp(first[0], "DISK-W") == 0){
		kprint("disk write:\n");
		//if(ata_write(100, 1, (u16 *)"hello\0") != 0) kprint("read failed\n");
		disk_w_text(first[1]);
	} else if(strcmp(first[0], "DISK-R") == 0){
		//u16 read[1024];
		kprint("disk read:\n");
		test[0] = '\0';
		test[1] = '\0';
		//if(ata_read(100, 1, test) != 0) kprint("read failed\n");
		disk_test_r();
		kprint((s8 *)test);
		kprint("\n");
	}
	kprint("you typed: ");
	kprint(str);
	//kprint(first);
	kprint("\n> ");
	return;
}

