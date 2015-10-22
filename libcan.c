#include "libcan.h"

void initcan(unsigned long canAddr, unsigned long resetAddr, unsigned char btr0, unsigned char btr1)
{
	
	int i;
	
	for (i=0; i<10; i++)
		;
	
	// give it a hardware reset
	*((unsigned char*)(resetAddr)) = 0x00;

	for (i=0; i<10000; i++)
		;

	*((unsigned char*)(resetAddr)) = 0xff;

	for (i=0; i<10000; i++)
		;
	
	// start to initialization
	*((unsigned char*)(canAddr + 0x00)) = 0x01; // set to reset mode

	for (i=0; i<10; i++)
		;

    *((unsigned char*)(canAddr + 0x7c)) = 0x80;

	for (i=0; i<10; i++)
		;
    
    *((unsigned char*)(canAddr + 0x20)) = 0xda;

	for (i=0; i<10; i++)
		;

    *((unsigned char*)(canAddr + 0x10)) = 0x00; // disable irq

	for (i=0; i<10; i++)
		;

    *((unsigned char*)(canAddr + 0x18)) = btr0; // btr0
    
    for (i=0; i<10; i++)
		;

    *((unsigned char*)(canAddr + 0x1c)) = btr1; // btr1

	for (i=0; i<10; i++)
		;

    *((unsigned char*)(canAddr + 0x40)) = 0x00; // acr
    
    for (i=0; i<10; i++)
		;

    *((unsigned char*)(canAddr + 0x44)) = 0x00;
    
    for (i=0; i<10; i++)
		;

    *((unsigned char*)(canAddr + 0x48)) = 0x00;
    
    for (i=0; i<10; i++)
		;

    *((unsigned char*)(canAddr + 0x4c)) = 0x00;

	for (i=0; i<10; i++)
		;

    *((unsigned char*)(canAddr + 0x50)) = 0xff; // amr
    
    for (i=0; i<10; i++)
		;

    *((unsigned char*)(canAddr + 0x54)) = 0xff;
    
    for (i=0; i<10; i++)
		;

    *((unsigned char*)(canAddr + 0x58)) = 0xff;
    
    for (i=0; i<10; i++)
		;

    *((unsigned char*)(canAddr + 0x5c)) = 0xff;

	for (i=0; i<10; i++)
		;

    *((unsigned char*)(canAddr + 0x00)) = 0x00; // set to operation mode

	for (i=0; i<10; i++)
		;
	
	*((unsigned char*)(canAddr + 0x10)) = 0xff; // enable irq

	for (i=0; i<10; i++)
		;

}

int sendtocan(unsigned long canAddr, unsigned char* buf)
{

	unsigned char mode, status;
	int i, j;
	
	for (i=0; i<10; i++)
		;

	mode = (*((unsigned char*)(canAddr + 0x00 * 4)));
	
	for (i=0; i<10; i++)
		;
	
	if (mode == 0x01)
		return 1;

	status = (*((unsigned char*)(canAddr + 0x02 * 4)));
	
	if (status & 0x04 != 0x04)
		return 2;

	for (i=0; i<10; i++)
		;
	
	for (j=0; j<11; j++) {
		
		*((unsigned char *)(canAddr + (16 + j) * 4)) = buf[j];
		
		for (i=0; i<10; i++)
			;

	}
	
	*((unsigned char *)(canAddr + 0x01 * 4)) = 0x01;
	
	for (i=0; i<10; i++)
		;
	
	return 0;
}

int recvfromcan(unsigned long canAddr, unsigned char* buf)
{
	
	int i, j;
	unsigned char mode, status;
	
	for (i=0; i<50; i++)
		;
	
	status = (*((unsigned char*)(canAddr + 0x02 * 4)));
	
	for (i=0; i<50; i++)
		;
	
	if ((status & 0x01) != 0) {
		
		for (j=0; j<11; j++) {
			
			buf[j] = (*((unsigned char*)(canAddr + (16 + j) * 4))) & 0xff;
			
			for (i=0; i<50; i++)
				;
		}
		
		*((unsigned char*)(canAddr + 0x01 * 4)) = 0x04;
		
		for (i=0; i<50; i++)
			;
	
		return 0;
	
	} else {
		
		return 1;
	}
	
}