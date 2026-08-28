
#include "kprint.h"


void kclear(){
	unsigned int j = 0;
	u8 *vidptr = (u8*)VIDEO_MEM;

	//clear screen:
	while(j<80*25*2){
		vidptr[j] = ' ';
		vidptr[j+1] = DEF_COLOR; // gray on black
		j = j + 2;
	}
}


// void kprint_char_at(volatile char *vidptr, unsigned char str, unsigned char x, unsigned char y, unsigned char col){
// 	unsigned int pos = (x + (y * 80)) * 2;
//
// }
