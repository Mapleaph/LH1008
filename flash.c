#include <stdio.h>
#include <csl_irq.h>
#include "6713_FLASH.h"
#include "LH_1008.h"

//#include "LH1111.h"
/********************************************************************************\
\* Flash erase function. *\
\********************************************************************************/
int Flash_Erase()
{

	unsigned long i;
	int sctnum;

	for(sctnum=0;sctnum<SECTOR_NUM;sctnum++)
	{
		write32(FLASH_BASE_ADDR+0x1554,0x00aa00aa);
		write32(FLASH_BASE_ADDR+0xaa8,0x00550055);
		write32(FLASH_BASE_ADDR+0x1554,0x00800080);
		write32(FLASH_BASE_ADDR+0x1554,0x00aa00aa);
		write32(FLASH_BASE_ADDR+0xaa8,0x00550055);
		//sector erase
		write32(FLASH_BASE_ADDR+sctnum*SECTOR_SIZE*4,0x00300030);
		i=10000;
		while(i>0)
		{i--;}
		while((read32(FLASH_BASE_ADDR+sctnum*SECTOR_SIZE*4) & 0xffffffff) != 0xffffffff);
			
	}
	return 1;
}

int Flash_Chip_Erase()
{

	unsigned long i;
	int sctnum;

	write32(FLASH_BASE_ADDR+0x1554, 0x00aa00aa);
	write32(FLASH_BASE_ADDR+0xaa8, 0x00550055);
	write32(FLASH_BASE_ADDR+0x1554, 0x00800080);
	write32(FLASH_BASE_ADDR+0x1554, 0x00aa00aa);
	write32(FLASH_BASE_ADDR+0xaa8, 0x00550055);
	write32(FLASH_BASE_ADDR+0x1554, 0x00100010);

	
	for(sctnum=0;sctnum<SECTOR_NUM;sctnum++) {
	
		i=10000;
		while(i>0)
		{i--;}
		while((read32(FLASH_BASE_ADDR+sctnum*SECTOR_SIZE*4) & 0xffffffff) != 0xffffffff);
	
	}

}
/********************************************************************************\
*  Write a single data. *\
********************************************************************************/
void Flash_Writes(unsigned long addr,unsigned long data)
{
	unsigned long wrchar;
	unsigned long realaddr;
	unsigned long i;
	
	wrchar = data & 0xffffffff;
	realaddr = addr & 0xffffffff;
	write32(FLASH_BASE_ADDR+0x555 * 4,0x00aa00aa);
	write32(FLASH_BASE_ADDR+0x2aa * 4,0x00550055);
	write32(FLASH_BASE_ADDR+0x555 * 4,0x00a000A0);
	write32(realaddr, wrchar);
	i=1000;
	while(i>0)
	{i--;}
//	while((( *( (unsigned char *)(realaddr) ) )&0xffffffff) != wrchar);
	
}


/********************************************************************************\
\* Write the certain length data. *\
\********************************************************************************/
void Flash_Writem(Uint32 addr,Uint32 *ptr,Uint32 length)
{
	Uint32 i;
	for(i=0; i<=length; i++)
	{
		Flash_Writes(addr+i*4,*(ptr+i));
	}
}

/********************************************************************************\
\* Read a single data. *\
\********************************************************************************/
unsigned char Flash_Reads(Uint32 addr)
{
	return (*(unsigned char *)addr);
}

/********************************************************************************\
\* Read the certain length data. *\
\********************************************************************************/
void Flash_Readm(Uint32 addr,Uint16 *ptr,Uint32 length)
{
	Uint32 i;
	for(i = 0; i < length; i++)
	{
		*(ptr + i) = Flash_Reads(addr+2*i);
	}
}

/********************************************************************************\
\* End of DEC6713_FLASH.C *\
\********************************************************************************/


