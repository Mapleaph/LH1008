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

void com_init()
{
	int i;

	*((unsigned char*)(0xa0051000+0x0C)) = 0x24;

	for (i=0; i<10000; i++)
		;

	*((unsigned char*)(0xa0050000+0x0C)) = 0x24;

	for (i=0; i<10000; i++)
		;

	*((unsigned char*)(0xa0050020+0x0C)) = 0x24;

	for (i=0; i<10000; i++)
		;
}

/*
 *  TODO
 */
void test_com()
{
/*    char quit[1];
    int i;

    do {
        disptitle("COM TEST");

        lineBegin();
        for (i=0; i<6; i++) printf(" ");
        printf("COM1-RS422    COM2-RS422   COM3-RS485");
        lineEnd(43);

        disp();

        lineBegin();
        for (i=0; i<6; i++) printf(" ");
        printf("1-COM_SEND    2-COM_RECE   3-COM_LOOP   0-quit");
        lineEnd(52);

        disp();

        while ((gets(quit)) == 0);
        switch (quit[0]) {

        case '1':
            //com_send();
            break;
        case '2':
            //com_receive();
            break;
        case '3':
            //com_loop();
            break;
        default:;

        }

    } while (quit[0] != '0');

    dispfoot();
	*/
}
