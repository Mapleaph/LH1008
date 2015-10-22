void sendtocom(unsigned long addr, unsigned char *buf, int size)
{
    unsigned char fifosize;
    int i, j;

	do {
	
		fifosize = (*((unsigned char *)(addr + 0x14)));
		for (j=0; j<10; j++)
			;
	
	} while ((256 - fifosize) < size);
		
	
    for (i=0; i<size; i++) {

		for (j=0; j<30000; j++)
			;
     
        *((unsigned char *)(addr + 0x0)) = buf[i];
    
    }

}

void sendtocom0(unsigned char* buf)
{
	
	sendtocom(0xa0051000, buf, strlen(buf));

}

unsigned int recfromcom(unsigned long addr, unsigned int read_data[])
{
    unsigned int fifosize;
    int i, j;

	fifosize = (*((unsigned int*)(addr + 0x10)));

	for (i=0; i<10; i++)
		;

	//printf("fifosize is 0x%x\n", fifosize);

	if (fifosize > 0) {
	
		for (j=0; j<20000; j++) // 1ms delay
			;

		read_data[0] = (*((unsigned char*)(addr + 0x0)));

		return 1;

	} else {

		return 0;
	}
}
