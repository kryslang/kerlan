#include "../cpu/types.h"


void memcpy(u32* point1, u32* point2, u32 size){
	u32 i = 0;
	for(i = 0; i < size; i++){
		point2[i] = point1[i];
	}
}

void memset(void *mem, u8 val, u32 len){
	u8 *mem_s = (u8 *)mem;
	while(len>0){
		*mem_s = val;
		mem_s++;
		len--;
	}
	//return mem;
}
