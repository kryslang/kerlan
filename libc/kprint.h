#ifndef SCREEN_H
#define SCREEN_H
#include "../cpu/types.h"

#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0f
#define RED_ON_WHITE 0xf4

/* Screen i/o ports */
#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5
#define VIDEO_MEM 0xb8000


#define DEF_COLOR 0x0f

void kclear();
void kprint(s8 *str);
void kprint_at(s8 *str, u8 x, u8 y,u8 col);
void print_backspace();

#endif
