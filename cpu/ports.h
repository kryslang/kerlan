#ifndef PORTS_H
#define PORTS_H

#include "../cpu/types.h"

unsigned char port_byte_in (u16 port);
void port_byte_out (u16 port, u8 data);
unsigned short port_word_in (u16 port);
void port_word_read(u16 port, void* addr, u32 count);
void port_word_write(u16 port, const void* addr, u32 count);
void port_word_out (u16 port, u16 data);

#endif
