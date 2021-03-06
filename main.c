#include "emif.h"
#include "csl.h"
#include "timer.h"
#include "LH_1008.h"
#include "csl_timer.h"
#include "csl_irq.h"
#include "stdio.h"
#include "libemv.h"
#include "libgps.h"
#include "libim.h"
#include "libcom.h"
#include "libcan.h"
#include "6713_FLASH.h"
#include "pll.h"

#define TESTCOUNT 0x9999

extern vectors();

void initboard();
void initsdram();
void test_entry();
void initmodules();

int main(void)
{                               
	const int flash_burn = 0;

	int testcpu;

	initboard();
	initsdram();
	com_init();
	
	sendtocom0("LH-1008 Satellite Navigation System\n\n\n");

	initmodules();

	//TestCPU
	testcpu = (0xabcddcba * 0x56899865 + 0xccdd) / 0xaabb;
	testcpu <<= 16;
	testcpu = ~testcpu;

	if (testcpu == 0x11DCFFFF) {

		sendtocom0("Satellite Navigation System Start\n\n");

	}

	//Test Flash
	if (flash_burn == 1) {

		Flash_Chip_Erase();
		printf("Flash Erase Complete\n");

		Flash_Writem(FLASH_BASE_ADDR, 0x000, 0x48);
		Flash_Writem(FLASH_BASE_ADDR+0x400, (Uint32*)0x400, 0x5b30);
		printf("Flash Write Complete\n");
	}

	test_entry();

/*
	printf("TEST SDRAMing... Write 0x00000000\t\n");

	for (i=0; i<0x1000000; i++)
		write16((0x80000000 + i*2), 0x0);

	for (i=0; i<0x1000000; i++) {

		tmptest = read16(0x80000000 + i*2);

		if (tmptest != 0) {

			printf("SDRAM Check failed...\t\n");
			printf("i=0x%x\r\n", i);
			printf("tmptest=0x%x\r\n", tmptest);
			printf("testram=0x%x\r\n", testram);
		}
	}

	printf("SDRAM Check 0x00000000 OK\t\n");

	printf("TEST SDRAMing... Write 0xFFFFFFFF\t\n");

	for (i=0; i<0x1000000; i++)
		write16((0x80000000 + i*2), 0xffff);

	for (i=0; i<0x1000000; i++) {

		tmptest = read16(0x80000000 + i*2);

		if (tmptest != 0xffff) {

			printf("SDRAM Check failed...\t\n");
			printf("i=0x%x\r\n", i);
			printf("tmptest=0x%x\r\n", tmptest);
			printf("testram=0x%x\r\n", testram);
		}
	}

	printf("SDRAM Check 0xFFFFFFFF OK\t\n");

	printf("TEST SDRAMing... Write 0xAAAAAAAA\t\n");

	for (i=0;i<0x1000000; i++)
		write16((0x80000000 + i*2), 0xaaaa);

	for (i=0; i<0x1000000; i++) {

		tmptest = read16(0x80000000 + i*2);

		if (tmptest != 0xaaaa) {

			printf("SDRAM Check failed...\t\n");
			printf("i=0x%x\r\n", i);
			printf("tmptest=0x%x\r\n", tmptest);
			printf("testram=0x%x\r\n", testram);

		}
	}

	printf("SDRAM Check 0xAAAAAAAA OK\t\n");

	printf("TEST SDRAMing... Write 0x55555555\t\n");

	for (i=0; i<0x1000000; i++)
		write16((0x80000000 + i*2), 0x5555);

	for (i=0; i<0x1000000; i++) {

		tmptest = read16(0x80000000 + i*2);

		if (tmptest != 0x5555) {

			printf("SDRAM Check failed...\t\n");
			printf("i=0x%x\r\n", i);
			printf("tmptest=0x%x\r\n", tmptest);
			printf("testram=0x%x\r\n", testram);

		}
	}

	printf("SDRAM Check 0x55555555 OK\t\n");

	printf("TEST SDRAMing...Write Accumulation \t\n");

	for (i=0, testram=TESTCOUNT; i<0x1000000; i++, testram--)
		write16((0x80000000 + i*2), testram);

	for (i=0, testram=TESTCOUNT; i<0x1000000; i++, testram--) {

		tmptest = read16(0x80000000 + i*2);

		if (tmptest != testram) {

			printf("SDRAM Check failed...\t\n");
			printf("i=0x%x\r\n", i);
			printf("tmptest=0x%x\r\n", tmptest);
			printf("testram=0x%x\r\n", testram);

		}
	}

	printf("SDRAM Test good\n");

	initsdram();
*/

}

void initboard()
{
	CSL_init();
	PLL_Init();
	EMIF_Init();
	TIMER_Init();

	IRQ_globalDisable();
}


/*
 * RS232 COM7: 0xa0051000~0xa005101f
 * RS232 COM1: 0xa0050000~0xa005001f
 * RS232 COM2: 0xa0050020~0xa005003f
 * RS422 COM3: 0xa0050040~0xa005005f
 * RS422 COM4: 0xa0050060~0xa005007f
 * RS422 COM5: 0xa0050080~0xa005009f
 * RS422 COM6: 0xa00500a0~0xa00500bf
 */


void test_entry()
{
	
	int selection, depth;
	int i, j;
	unsigned char readout;

	selection = -1;
	depth = -1;

	while (1) {

		sendtocom0("****************\n");
		sendtocom0("SYSTEM TEST MENU\n");
		sendtocom0("****************\n\n");


		for (i=0; i<3; i++) sendtocom0(" ");
		sendtocom0("[01]   GPS TEST\n");

		for (i=0; i<3; i++) sendtocom0(" ");
		sendtocom0("[02]   IMU TEST\n");

		for (i=0; i<3; i++) sendtocom0(" ");
		sendtocom0("[03]   CAN TEST\n");

		for (i=0; i<3; i++) sendtocom0(" ");
		sendtocom0("[04] RS422 TEST\n");

		for (i=0; i<3; i++) sendtocom0(" ");
		sendtocom0("[05]   EMV TEST\n");
		sendtocom0("\n");

		depth = *((unsigned char*)DBGADDR+0x10);

		for (j=0; j<10; j++)
			;

		if (depth > 0) {

			for (i=0; i<depth; i++) {

				readout = *((unsigned char*)DBGADDR);

				for (j=0; j<10; j++)
					;

			}

		}

		sendtocom0("Please Enter Your Choice:\n\n");

		for (j=0; j<20000000; j++)
			;
	
		do {

			depth = *((unsigned char*)DBGADDR+0x10);
			for (j=0; j<10; j++)
				;

			if (depth > 0) {

				selection = *((unsigned char*)DBGADDR);
				for (j=0; j<10; j++)
					;
				break;
			}

		} while (selection == 0 || depth == 0);


		for (j=0; j<20000000; j++)
			;

		switch (selection) {
		case 1:
			test_gps();
			break;
		case 2:
			test_im();
			break;
		case 3:
			test_can();
			break;
		case 4:
			test_com();
			break;
		case 5:
			test_EMV();
			break;
		}
	}
}

void initmodules()
{

	int i;

	sendtocom0("Initializing the System\n");

	for (i=0; i<2000000; i++)
		;

	sendtocom0(".");
	for (i=0; i<2000000; i++)
		;

	sendtocom0(".");
	for (i=0; i<2000000; i++)
		;

	sendtocom0(".\n\n");
	for (i=0; i<2000000; i++)
		;

	sendtocom0("GPS Module...");
	for (i=0; i<2000000; i++)
		;
	sendtocom0("Done.\n");


	//im_init();
	sendtocom0("IMU Module...");
	for (i=0; i<2000000; i++)
		;
	sendtocom0("Done.\n");

	sendtocom0("CAN Module...");
	for (i=0; i<2000000; i++)
		;
	sendtocom0("Done.\n");

	init_EMV();
	close_EMV_All();
	sendtocom0("EMV Module...");
	for (i=0; i<2000000; i++)
		;
	sendtocom0("Done.\n\n\n");

}

void initsdram()
{
	int i;
	for(i=0; i<0x1000000; i++)
		write16((0x80000000 + i*2), 0x0);
}
