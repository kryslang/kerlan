
#include "kprint.h"
#include "../cpu/ports.h"


void kprint_char_at(u8 str, u8 x, u8 y, u8 col);
int get_cursor_offset();
void set_cursor_offset(int offset);




//public
void kclear(){
	u16 j = 0;
	u8 *vidptr = (u8*)VIDEO_MEM;

	//clear screen:
	while(j<80*25*2){
		vidptr[j] = ' ';
		vidptr[j+1] = DEF_COLOR; // white on black
		j = j + 2;
	}
	set_cursor_offset(0);
}
void kprint_at(s8 *str, u8 x, u8 y,u8 col){

	u8 i = 0;


	while(str[i]!='\0'){
		if(x>=MAX_COLS){
			x = 0;
			y++;

		}
		if(str[i]=='\n'){
			x = 0;
			y++;
			i++;
			continue;
		}
		kprint_char_at(str[i], x, y, col);
		x++;

		i++;
	}
	set_cursor_offset((x + (y * 80)) * 2);

}

void kprint(s8 *str){
	u16 off = get_cursor_offset() / 2;
	u8 x = off % 80;
	u8 y = (off - x) / 80;

	kprint_at(str, x,  y, DEF_COLOR);

}




//private
void kprint_char_at(u8 str, u8 x, u8 y, u8 col){
	u8 *vidptr = (u8*)VIDEO_MEM;
	u16 pos = (x + (y * 80)) * 2;
	vidptr[pos] = str;
	vidptr[pos+1] = col;

}












// cursor
int get_cursor_offset() {
	/* Use the VGA ports to get the current cursor position
	 * 1. Ask for high byte of the cursor offset (data 14)
	 * 2. Ask for low byte (data 15)
	 */
	port_byte_out(REG_SCREEN_CTRL, 14);
	int offset = port_byte_in(REG_SCREEN_DATA) << 8; /* High byte: << 8 */
	port_byte_out(REG_SCREEN_CTRL, 15);
	offset += port_byte_in(REG_SCREEN_DATA);
	return offset * 2; /* Position * size of character cell */
}

void set_cursor_offset(int offset) {
	/* Similar to get_cursor_offset, but instead of reading we write data */
	offset /= 2;
	port_byte_out(REG_SCREEN_CTRL, 14);
	port_byte_out(REG_SCREEN_DATA, (u8)(offset >> 8));
	port_byte_out(REG_SCREEN_CTRL, 15);
	port_byte_out(REG_SCREEN_DATA, (u8)(offset & 0xff));
}
