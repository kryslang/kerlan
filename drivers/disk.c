
#include "../cpu/ports.h"
#include "./disk.h"

// Standard ATA I/O Ports for the Primary Bus
#define ATA_REG_DATA       0x1F0
#define ATA_REG_SECCOUNT   0x1F2
#define ATA_REG_LBA_LOW    0x1F3
#define ATA_REG_LBA_MID    0x1F4
#define ATA_REG_LBA_HIGH   0x1F5
#define ATA_REG_DRIVE      0x1F6
#define ATA_REG_COMMAND    0x1F7
#define ATA_REG_STATUS     0x1F7
#define ATA_REG_ALTSTATUS  0x3F6   /* also the device control register when written */

#define ATA_CMD_READ_PIO   0x20
#define ATA_CMD_WRITE_PIO  0x30
#define ATA_CMD_FLUSH      0xE7

#define ATA_SR_ERR 0x01
#define ATA_SR_DRQ 0x08
#define ATA_SR_DF  0x20
#define ATA_SR_BSY 0x80


static int ata_wait_tran(){
	u8 i;
	u8 s;
	for(i = 0; i<4; i++){
		port_byte_in(ATA_REG_ALTSTATUS); // this is the wierdest delay i ever seen, AI told me to do this, it is about 400ns
	}
	while(1){
		s = port_byte_in(ATA_REG_STATUS);
		if(s & ATA_SR_BSY){
			continue;
		}
		if(s & (ATA_SR_ERR | ATA_SR_DF)){
			return -1;
		}
		if(s & ATA_SR_DRQ){
			return 0;
		}
	}
}

static int ata_wait_busy(){
	u8 i;
	u8 s;
	for(i = 0; i<4; i++){
		port_byte_in(ATA_REG_ALTSTATUS); // this is the wierdest delay i ever seen, AI told me to do this, it is about 400ns
	}
	while(1){
		s = port_byte_in(ATA_REG_STATUS);
		if(s & ATA_SR_BSY){
			continue;
		}
		if(s & (ATA_SR_ERR | ATA_SR_DF)){
			return -1;
		}
		return 0;
	}
}


static void ata_setup(u32 lba, u8 count, u8 command){
	port_byte_out(ATA_REG_DRIVE, 0xE0 | ((lba>>24) & 0x0F)  ); // 0xE0 = main drive, | = OR, the highest 4 bits of lba specify what drive to read from
	port_byte_out(ATA_REG_SECCOUNT, count); //send the number of sectors to read

	port_byte_out(ATA_REG_LBA_LOW, (u8)(lba & 0xFF)); // this is bit operation magic, it is genious (ai told me :]  )
	port_byte_out(ATA_REG_LBA_MID, (u8)((lba >> 8) & 0xFF));
	port_byte_out(ATA_REG_LBA_HIGH, (u8)((lba >> 16) & 0xFF));



	port_byte_out(ATA_REG_COMMAND, command); // this sends the command to read the data to the drive
}



/* Reads sectors from the Primary ATA Master drive using 28-bit LBA.
*
* @param lba    28-bit Logical Block Address (max sector ~137 GB)
* @param count  Number of sectors to read (0 means 256 sectors)
* @param target Pointer to the 32-bit destination memory buffer
*/

int ata_read(u32 lba, u8 count, u16 *target){
	ata_setup(lba, count, ATA_CMD_READ_PIO);

	u32 number_of_sectors;
	if(count==0){
		number_of_sectors = 256;
	} else {
		number_of_sectors = count;
	}
	//u8 status;
	u32 i;
	for(i = 0; i<number_of_sectors; i++){
		if(ata_wait_tran() != 0) {
			return -1;
		}
		port_word_read(ATA_REG_DATA, target + (i * 256), 256);
	}
	return 0;
}


int ata_write(u32 lba, u8 count, u16 *source){
	ata_setup(lba, count, ATA_CMD_WRITE_PIO);

	u32 number_of_sectors;
	if(count==0){
		number_of_sectors = 256;
	} else {
		number_of_sectors = count;
	}
	//u8 status;
	u32 i;
	for(i = 0; i<number_of_sectors; i++){
		if(ata_wait_tran() != 0) {
			return -1;
		}
		port_word_write(ATA_REG_DATA, source + (i * 256), 256);
	}
	return ata_wait_busy();
}

