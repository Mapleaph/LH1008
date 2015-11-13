#include "libcan.h"
#include <stdio.h>
#include <stdlib.h>

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

void test_can()
{

	unsigned char mode, status;
	unsigned char sendbuf[11], recvbuf[11];
	unsigned char sendstr[11];
	unsigned char recvstr[11];
	unsigned char txcntstr[10];
	unsigned char txerrcntstr[10];
	unsigned char rxcntstr[10];
	unsigned char rxerrcntstr[10];

	int i, j, k;

	int ret;

	int txcnt = 0;
	int txerrcnt = 0;
	int rxcnt = 0;
	int rxerrcnt = 0;

	unsigned long canAddr1 = 0xa0052800;
	unsigned long resetAddr1 = 0xa00527f8;

	unsigned long canAddr2 = 0xa0052c00;
	unsigned long resetAddr2 = 0xa00527fc;

	initcan(canAddr2, resetAddr2, 0x00, 0x14);

	for (i=0; i<100000; i++)
		;

	initcan(canAddr1, resetAddr1, 0x00, 0x14);

	sendbuf[0] = 0x08;
	sendbuf[1] = 0x00;
	sendbuf[2] = 0x00;
	sendbuf[3] = 0x01;
	sendbuf[4] = 0x02;
	sendbuf[5] = 0x03;
	sendbuf[6] = 0x04;
	sendbuf[7] = 0x05;
	sendbuf[8] = 0x06;
	sendbuf[9] = 0x07;
	sendbuf[10] = 0x08;

	sendtocom0("**************\n");
	sendtocom0("CAN TEST BEGIN\n");
	sendtocom0("**************\n\n");


	//disptitle("CAN TEST");
	//disp();

	//lineBegin();
	for (j=0; j<6; j++) sendtocom0(" ");
	sendtocom0("******************************\n");
	for (j=0; j<6; j++) sendtocom0(" ");
	sendtocom0("CAN1 to CAN2 Test (Five Times)\n");
	for (j=0; j<6; j++) sendtocom0(" ");
	sendtocom0("******************************\n\n");

	//lineEnd(30);


	for (i=0; i<5; i++) {

		if (sendtocan(canAddr1, sendbuf) == 0) {

			txcnt++;

			for (j=0; j<6; j++) sendtocom0(" ");

			sendtocom0("CAN1 Send: ");

			for (k=3; k<11; k++) {

				sprintf(sendstr, "0x%x", sendbuf[k]);

				sendtocom0(sendstr);
				sendtocom0(" ");

			}

			for (j=3; j<11; j++) {
				sendbuf[j]++;
			}

			sendtocom0("\n");


		} else {

			txerrcnt++;

		}

		for (j=0; j<200000; j++)
			;

		if (recvfromcan(canAddr2, recvbuf) == 0) {

			rxcnt++;

			//for (j=0; j<11; j++)
			//	printf("recvbuf[%d] is 0x%x\n", j, recvbuf[j]);
			for (j=0; j<6; j++) sendtocom0(" ");
			sendtocom0("CAN2 Recv: ");
			for (k=3; k<11; k++) {

				sprintf(recvstr, "0x%x", recvbuf[k]);

				sendtocom0(recvstr);
				sendtocom0(" ");

			}

			sendtocom0("\n");

		} else {

			rxerrcnt++;
			for (j=0; j<6; j++) sendtocom0(" ");
			sendtocom0("CAN2 Recv: ");
			sendtocom0("\n");

		}

		for (j=0; j<20000000; j++) // delay for about 1s
			;

		sendtocom0("\n");
	}


	//printf("txcnt is %d, txerrcnt is %d, rxcnt is %d, rxerrcnt is %d\n", txcnt, txerrcnt, rxcnt, rxerrcnt);

	sprintf(txcntstr, "%d", txcnt);
	sprintf(txerrcntstr, "%d", txerrcnt);
	sprintf(rxcntstr, "%d", rxcnt);
	sprintf(rxerrcntstr, "%d", rxerrcnt);

	for (j=0; j<6; j++) sendtocom0(" ");
	sendtocom0("txcnt is ");
	sendtocom0(txcntstr);
	sendtocom0(", ");

	sendtocom0("txerrcnt is ");
	sendtocom0(txerrcntstr);
	sendtocom0(", ");

	sendtocom0("rxcnt is ");
	sendtocom0(rxcntstr);
	sendtocom0(", ");

	sendtocom0("rxerrcnt is ");
	sendtocom0(rxerrcntstr);


	txcnt = 0;
	rxcnt = 0;
	txerrcnt = 0;
	rxerrcnt = 0;

	sendtocom0("\n\n\n");
	for (j=0; j<6; j++) sendtocom0(" ");
	sendtocom0("******************************\n");
	for (j=0; j<6; j++) sendtocom0(" ");
	sendtocom0("CAN2 to CAN1 Test (Five Times)\n");
	for (j=0; j<6; j++) sendtocom0(" ");
	sendtocom0("******************************\n\n");

	for (i=0; i<5; i++) {

		if (sendtocan(canAddr1, sendbuf) == 0) {

			txcnt++;
			for (j=0; j<6; j++) sendtocom0(" ");
			sendtocom0("CAN2 Send: ");
			for (k=3; k<11; k++) {

				sprintf(sendstr, "0x%x", sendbuf[k]);

				sendtocom0(sendstr);
				sendtocom0(" ");

			}

			for (j=2; j<11; j++) {
				sendbuf[j]++;
			}

			sendtocom0("\n");

		} else {

			txerrcnt++;

		}

		for (j=0; j<200000; j++)
			;

		if (recvfromcan(canAddr2, recvbuf) == 0) {

			rxcnt++;

			//for (j=0; j<11; j++)
			//	printf("recvbuf[%d] is 0x%x\n", j, recvbuf[j]);
			for (j=0; j<6; j++) sendtocom0(" ");
			sendtocom0("CAN1 Recv: ");
			for (k=3; k<11; k++) {

				sprintf(recvstr, "0x%x", recvbuf[k]);

				sendtocom0(recvstr);
				sendtocom0(" ");

			}

			sendtocom0("\n");

		} else {

			rxerrcnt++;
			for (j=0; j<6; j++) sendtocom0(" ");
			sendtocom0("CAN1 Recv: ");
			sendtocom0("\n");

		}

		for (j=0; j<20000000; j++) // delay for about 1s
			;

		sendtocom0("\n");
	}

	//printf("txcnt is %d, txerrcnt is %d, rxcnt is %d, rxerrcnt is %d\n", txcnt, txerrcnt, rxcnt, rxerrcnt);
	//printf("done\n");

	sprintf(txcntstr, "%d", txcnt);
	sprintf(txerrcntstr, "%d", txerrcnt);
	sprintf(rxcntstr, "%d", rxcnt);
	sprintf(rxerrcntstr, "%d", rxerrcnt);

	for (j=0; j<6; j++) sendtocom0(" ");
	sendtocom0("txcnt is ");
	sendtocom0(txcntstr);
	sendtocom0(", ");

	sendtocom0("txerrcnt is ");
	sendtocom0(txerrcntstr);
	sendtocom0(", ");

	sendtocom0("rxcnt is ");
	sendtocom0(rxcntstr);
	sendtocom0(", ");

	sendtocom0("rxerrcnt is ");
	sendtocom0(rxerrcntstr);
	sendtocom0(" ");

	sendtocom0("\n\n");

	sendtocom0("************\n");
	sendtocom0("CAN TEST END\n");
	sendtocom0("************\n\n");

}
