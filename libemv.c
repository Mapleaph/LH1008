#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "LH_1008.h"
#include "libemv.h"

// 12 Electromagnetic Valves - GPIO Mapping Array
//static int switch2gpio[12] = { 5, 6, 7, 15, 14, 13, 12, 11, 10, 9, 8, 3 };
//static int gpio2switch[16] = { -1, -1, -1, 11, -1, 0, 1, 2, 10, 9, 8, 7, 6, 5, 4, 3 };

/*
	GPIO_VAL_REG value list for 12 valves

	1 : 0x0020
	2 : 0x0040
	3 : 0x0080
	4 : 0x8000
	5 : 0x4000
	6 : 0x2000
	7 : 0x1000
	8 : 0x0800
	9 : 0x0400
	10 : 0x0200
	11 : 0x0100
	12 : 0x0008
*/

void open_EMV(int n) // open the n-th EMV (1~12)
{

	int i;
	unsigned long val;

	val = read32(GPIO_VAL_REG);

	write32(GPIO_VAL_REG, val | (1<<switch2gpio[n-1]));

	for (i=0; i<10; i++)
		;

}

void close_EMV(int n) // close the n-th EMV (1~12)
{

	int i;
	unsigned long val;

	val = read32(GPIO_VAL_REG);

	write32(GPIO_VAL_REG, ~(~val | (1<<switch2gpio[n-1])));

	for (i=0; i<10; i++)
		;

}

/*
 * get_EMV()
 *
 * returns a 12-bit integer represents the 12 valves
 * each bit(0~11) represents one valve(1~12)
 * for example, if we got the result like 0x00c,
 * it means that the 3rd and 4th valves are now open
 * (the 2nd and 3rd bit is set),
 * while other 10 valves closed(the reset of bits are cleared).
 *
 */

int get_EMV()
{

	int val, ret=0;
	int i, j;

	val = read32(GPIO_VAL_REG);

	for (i=0; i<10; i++)
		;

	for (i=0; i<16; i++) {

		j = 1;

		if ((val & (j<<i)) && (gpio2switch[i] != -1)) {

			ret = ret | (1<<gpio2switch[i]);

		}
	}

	return ret;

}

/*
 * void set_EMV(int val)
 *
 * parameter val is a 12-bit integer represents the 12 valves
 * each bit(0~11) represents one valve(1~12)
 * for example, if we set the val to 0x00c,
 * it means that the 3rd and 4th valves will be opened
 * (the 2nd and 3rd bit is set),
 * while other 10 valves are set to closed
 * (the reset of bits are cleared).
 *
 */

void set_EMV(int val)
{

	int ret = 0, i, j;

	for (i=0; i<12; i++) {

		j = 1;

		if (val & (j<<i)) {

			ret = ret | (1<<switch2gpio[i]);

		}
	}

	write32(GPIO_VAL_REG, ret);

	for (i=0; i<10; i++)
		;

}

void open_EMV_All()
{
	int i;
	write32(GPIO_VAL_REG, 0xffff);
	for (i=0; i<10; i++)
		;
}

void close_EMV_All()
{
	int i;
	write32(GPIO_VAL_REG, 0x0000);
	for (i=0; i<10; i++)
		;
}

void init_EMV() // initialization for all valves
{
	int i;

	write32(GPIO_EN_REG, 0xffff);  // enable all the valves

	for (i=0; i<10; i++)
		;

	write32(GPIO_DIR_REG, 0xffff); // set the valves to output mode

	for (i=0; i<10; i++)
		;
}

void test_EMV()
{

	int i, j;
	char str[3];

	sendtocom0("**************\n");
	sendtocom0("EMV TEST BEGIN\n");
	sendtocom0("**************\n\n");

	for (i=0; i<6; i++) sendtocom0(" ");
	sendtocom0("**********************************\n");
	for (i=0; i<6; i++) sendtocom0(" ");
	sendtocom0("Open-Close One by One from 1 to 12\n");
	for (i=0; i<6; i++) sendtocom0(" ");
	sendtocom0("**********************************\n\n");

	// open-close one by one

	for (i=1; i<=12; i++) {

		for (j=0; j<6; j++) sendtocom0(" ");
		sendtocom0(" open valve ");
		sprintf(str, "%d", i);
		sendtocom0(str);
		sendtocom0("...\n");

		open_EMV(i);

		for (j=0; j<40000000; j++)
			;

		for (j=0; j<6; j++) sendtocom0(" ");
		sendtocom0("close valve ");
		sprintf(str, "%d", i);
		sendtocom0(str);
		sendtocom0("...\n\n");
		close_EMV(i);

		for (j=0; j<40000000; j++)
			;

	}

	sendtocom0("************\n");
	sendtocom0("EMV TEST END\n");
	sendtocom0("************\n\n");

	close_EMV_All();

}
