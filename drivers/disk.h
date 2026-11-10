#ifndef DISK_H
#define DISK_H



int ata_read(u32 lba, u8 count, u16 *target);
int ata_write(u32 lba, u8 count, u16 *source);

#endif
