#ifndef LH_1008_h
#define LH_1008_h

#define SECTOR_NUM          15
#define	SECTOR_SIZE	        0x10000
#define SDRAM_BASE_ADDR     0x80000000
#define FLASH_BASE_ADDR	    0x90000000
#define BASEDualRAM         0xa0280000 



#define GPIO_EN_REG		0x01B00000
#define GPIO_DIR_REG	0x01B00004
#define GPIO_VAL_REG	0x01B00008
#define GPIO_POL_REG    0x01B00024

#define read8(ADDRESS)          ((*((unsigned char *)(ADDRESS)))&0xff)
#define read16(ADDRESS)        	((*((unsigned short *)(ADDRESS)))&0xffff)
#define read32(ADDRESS)        	((*((unsigned int *)(ADDRESS)))&0xffffffff)

#define write8(ADDRESS,DATA)    (*((unsigned char *)(ADDRESS)))=(unsigned char)(DATA&0xff) 
#define write16(ADDRESS,DATA)   (*((unsigned short *)(ADDRESS)))=(unsigned short)(DATA&0xffff) 
#define write32(ADDRESS,DATA)   (*((unsigned int *)(ADDRESS)))=(unsigned int)(DATA & 0xffffffff) 

#define TESTADD_DUAL BASEDualRAM + 0x3000 
#define TEST_ADD BASEDualRAM + 0x300c
#define EX_INTERFACE_ADD	0xA0002AA8
#define EX_INTERFACE_TEST	0xA0001554
#define BAOMI_CARD_ADD		0xB03FEAA8
#define BAOMI_CARD_TEST		0xB0001554
#define ALL_FUNCIONADD		BASEDualRAM + 0x3020

#endif
  


