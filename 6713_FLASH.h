/********************************************************************************\
\*DEC6713_FLASH.H	  	v1.00	     							                            *
* Copyright	2004 by SEED Electronic Technology Ltd.
* All rights reserved. Property of SEED Electronic Technology Ltd.			                *
* Designed by:	Hongshuai.Li								                                *
\********************************************************************************/

#define		FLASH_UL1	0xAA
#define		FLASH_UL2	0x55
#define		FLASH_UL3	0x80
#define		FLASH_UL4	0xAA
#define		FLASH_UL5	0x55
#define		FLASH_SECTOR_UL6	0x30
#define		FLASH_CHIP_UL6		0x10
#define		FLASH_PROGRAM	0xA0

#define		CHIP_SIZE	0x80000
#define     FLASH_555   0x555
#define     FLASH_2AA   0x2aa

//volatile Uint16 *FLASH_5555 = (volatile Uint16 *) (0x90000000+(0x5555<<1));
//volatile Uint16 *FLASH_2AAA = (volatile Uint16 *) (0x90000000+(0x2AAA<<1));
//volatile unsigned char *FLASH_5555 = (volatile unsigned char *) (0x90000000+0x555);
//volatile unsigned char *FLASH_2AAA = (volatile unsigned char *) (0x90000000+0x2AA);
/********************************************************************************/
int Flash_Erase();
void Flash_Readm(Uint32 addr,Uint16 *ptr,Uint32 length);
unsigned char Flash_Reads(Uint32 addr);
void Flash_Writem(Uint32 addr,Uint32 *ptr,Uint32 length);
void Flash_Writes(unsigned long addr,unsigned long data);
void Flash_Chip_Erase();
