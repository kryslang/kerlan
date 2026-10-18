
#include "kprint.h"
#include "mem.h"
#include "../cpu/types.h"
#include "../cpu/ports.h"


int kprint_char_at(u8 str, u8 x, u8 y, u8 col);
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
	u16 off = 0;


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
		off = kprint_char_at(str[i], x, y, col);
		x = off % 80;
		y = (off - x) / 80;
		x++;

		i++;
	}
	set_cursor_offset((x + (y * 80)) * 2);

}




void print_backspace(){
	u16 off = (get_cursor_offset() / 2) - 1;
	u8 x = off % 80;
	u8 y = (off - x) / 80;

	off = kprint_char_at(' ', x,  y, DEF_COLOR);
	set_cursor_offset(off*2);
}


void kprint(s8 *str){
	u16 off = get_cursor_offset() / 2;
	u8 x = off % 80;
	u8 y = (off - x) / 80;

	kprint_at(str, x,  y, DEF_COLOR);


}




//private
int kprint_char_at(u8 str, u8 x, u8 y, u8 col){
	u8 *vidptr = (u8*)VIDEO_MEM;
	u16 pos = (x + (y * 80)) * 2;
	while(pos > 80*25*2){
		memcpy((u32*)(VIDEO_MEM + 80*2), (u32*)VIDEO_MEM, 80*24*2);
		pos -= 80*2;
	}
	vidptr[pos] = str;
	vidptr[pos+1] = col;
	return (u16)pos/2;
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
