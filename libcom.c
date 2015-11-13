#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "LH_1008.h"
#include "libcom.h"

void sendtocom(unsigned long addr, unsigned char* buf, int size)
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
	
	sendtocom(DBGADDR, buf, strlen(buf));

}

unsigned int recfromcom(unsigned long addr, unsigned char read_data[])
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

void set_loop_mode()
{
	int i;

	*((unsigned char*)(0xa0050040+0x08)) = 0x18; // COM3 MCR to loop mode

	for (i=0; i<10000; i++)
		;

	*((unsigned char*)(0xa0050060+0x08)) = 0x18; // COM4 MCR to loop mode

	for (i=0; i<10000; i++)
		;

	*((unsigned char*)(0xa0050080+0x08)) = 0x18; // COM5 MCR to loop mode

	for (i=0; i<10000; i++)
		;

	*((unsigned char*)(0xa00500a0+0x08)) = 0x18; // COM6 MCR to loop mode

	for (i=0; i<10000; i++)
		;

}

void set_normal_mode()
{
	int i;

	*((unsigned char*)(0xa0050040+0x08)) = 0x00; // COM3 MCR to loop mode

	for (i=0; i<10000; i++)
		;

	*((unsigned char*)(0xa0050060+0x08)) = 0x00; // COM4 MCR to loop mode

	for (i=0; i<10000; i++)
		;

	*((unsigned char*)(0xa0050080+0x08)) = 0x00; // COM5 MCR to loop mode

	for (i=0; i<10000; i++)
		;

	*((unsigned char*)(0xa00500a0+0x08)) = 0x00; // COM6 MCR to loop mode

	for (i=0; i<10000; i++)
		;

}

void com_init()
{
	int i;

	*((unsigned char*)(0xa0051000+0x0c)) = 0x24; // 232-COM7

	for (i=0; i<10000; i++)
		;

	*((unsigned char*)(0xa0050000+0x0c)) = 0x24; // 232-COM1

	for (i=0; i<10000; i++)
		;

	*((unsigned char*)(0xa0050020+0x0c)) = 0x24; // 232-COM2

	for (i=0; i<10000; i++)
		;

	*((unsigned char*)(0xa0050040+0x0c)) = 0x24; // 422-COM3

	for (i=0; i<10000; i++)
		;

	*((unsigned char*)(0xa0050060+0x0c)) = 0x24; // 422-COM4

	for (i=0; i<10000; i++)
		;

	*((unsigned char*)(0xa0050080+0x0c)) = 0x24; // 422-COM5

	for (i=0; i<10000; i++)
		;

	*((unsigned char*)(0xa00500a0+0x0c)) = 0x24; // 422-COM6

	for (i=0; i<10000; i++)
		;

}

void test_com()
{

	int i;

	set_loop_mode();

	sendtocom0("*************************\n");
	sendtocom0("RS422 LOOPBACK TEST BEGIN\n");
	sendtocom0("*************************\n\n");


	for (i=0; i<6; i++) sendtocom0(" ");
	sendtocom0("*****************\n");
	for (i=0; i<6; i++) sendtocom0(" ");
	sendtocom0("COM3 Test (Twice)\n");
	for (i=0; i<6; i++) sendtocom0(" ");
	sendtocom0("*****************\n\n");

	test_com_wf(COM3ADDR);

	for (i=0; i<6; i++) sendtocom0(" ");
	sendtocom0("*****************\n");
	for (i=0; i<6; i++) sendtocom0(" ");
	sendtocom0("COM4 Test (Twice)\n");
	for (i=0; i<6; i++) sendtocom0(" ");
	sendtocom0("*****************\n\n");

	test_com_wf(COM4ADDR);

	for (i=0; i<6; i++) sendtocom0(" ");
	sendtocom0("*****************\n");
	for (i=0; i<6; i++) sendtocom0(" ");
	sendtocom0("COM5 Test (Twice)\n");
	for (i=0; i<6; i++) sendtocom0(" ");
	sendtocom0("*****************\n\n");

	test_com_wf(COM5ADDR);

	for (i=0; i<6; i++) sendtocom0(" ");
	sendtocom0("*****************\n");
	for (i=0; i<6; i++) sendtocom0(" ");
	sendtocom0("COM6 Test (Twice)\n");
	for (i=0; i<6; i++) sendtocom0(" ");
	sendtocom0("*****************\n\n");

	test_com_wf(COM6ADDR);

	set_normal_mode();

	sendtocom0("***********************\n");
	sendtocom0("RS422 LOOPBACK TEST END\n");
	sendtocom0("***********************\n\n");

}

void test_com_wf(unsigned long comAddr)
{
	unsigned char sendbuf[5], recvbuf[1];
	unsigned char sendstr[5];
	unsigned char recvstr[11];

	int i, j, k;

	int ret;

	sendbuf[0] = 0x01;
	sendbuf[1] = 0x02;
	sendbuf[2] = 0x03;
	sendbuf[3] = 0x04;
	sendbuf[4] = 0x05;

	for (i=0; i<2; i++) {

		for (j=0; j<6; j++) sendtocom0(" ");

		sendtocom0("Send: ");

		sendtocom(comAddr, sendbuf, 5);

		for (k=0; k<5; k++) {

			sprintf(sendstr, "0x%x", sendbuf[k]);
			sendtocom0(sendstr);
			sendtocom0(" ");
			sendbuf[k]++;

		}

		sendtocom0("\n");

		for (j=0; j<6; j++) sendtocom0(" ");
		sendtocom0("Recv: ");

		for (k=0; k<5; k++) {

			if (recfromcom(comAddr, recvbuf) != 0) {

				sprintf(recvstr, "0x%x", recvbuf[0]);

				sendtocom0(recvstr);
				sendtocom0(" ");

			}
		}


		sendtocom0("\n");

		for (j=0; j<10000000; j++) // delay for about 1s
			;

		sendtocom0("\n");
	}
}
