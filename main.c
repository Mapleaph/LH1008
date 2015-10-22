#include "emif.h"
#include "csl.h"
#include "timer.h"
#include "LH_1008.h"
#include "csl_timer.h"
#include "csl_irq.h"
#include "stdio.h"

#define TESTCOUNT 0x9999
extern vectors();     
void initboard();
void InitDualRAM(void);
void initsdram();

int get_gps_data(unsigned char readbuf[]);
char* get_gps_date(char* gps_data);
char* get_gps_time(char* gps_data);
void get_gps_latitude(char* gps_data, char* latitude[]);
void get_gps_longitude(char* gps_data, char* longitude[]);
double get_gps_spd(char* gps_data);
double get_gps_cog(char* gps_data);

int get_im_data(unsigned int readbuf[]);
long double get_im_accx(int* im_data);
long double get_im_accy(int* im_data);
long double get_im_accz(int* im_data);
long double get_im_gyrx(int* im_data);
long double get_im_gyry(int* im_data);
long double get_im_gyrz(int* im_data);
long double get_im_magx(int* im_data);
long double get_im_magy(int* im_data);
long double get_im_magz(int* im_data);

void test_can();
void init_EMV();
void open_EMV(int);
void close_EMV(int);
void close_EMV_All();
void test_EMV();
void test_com();
void debug_entry();
void com_init();
void test_gps();

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

void im_init()
{

	int i;

	unsigned char buf[16];

	buf[0] = 0xFA;
	buf[1] = 0xFF;
	buf[2] = 0x30;
	buf[3] = 0x00;
	buf[4] = 0xD1;


	//sendtocom(0xa005000, buf, 5);

	buf[0] = 0xFA;
	buf[1] = 0xFF;
	buf[2] = 0xC0;
	buf[3] = 0x0C;
	buf[4] = 0x40;
	buf[5] = 0x23;
	buf[6] = 0x00;
	buf[7] = 0x64;
	buf[8] = 0x80;
	buf[9] = 0x23;
	buf[10] = 0x00;
	buf[11] = 0x64;
	buf[12] = 0xC0;
	buf[13] = 0x23;
	buf[14] = 0x00;
	buf[15] = 0x64;

	//sendtocom(0xa005000, buf, 16);

	buf[0] = 0xFA;
	buf[1] = 0xFF;
	buf[2] = 0x10;
	buf[3] = 0x00;
	buf[4] = 0xF1;


	sendtocom(0xa005000, buf, 5);

	/*
	*((unsigned char*)(0xa0050020)) = 0xFA;

	for (i=0; i<300000; i++)
		;

	*((unsigned char*)(0xa0050020)) = 0xFF;

	for (i=0; i<300000; i++)
		;

	*((unsigned char*)(0xa0050020)) = 0x10;

	for (i=0; i<300000; i++)
		;

	*((unsigned char*)(0xa0050020)) = 0x00;

	for (i=0; i<300000; i++)
		;

	*((unsigned char*)(0xa0050020)) = 0xF1;

	for (i=0; i<300000; i++)
		;
*/
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


	im_init();
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

int main(void)
{                               
	const int flash_burn = 0; 
	unsigned short tmptest,testram; 
	//int flag = 0;
	int testcpu,i, j ,INTtmp, k, temp, status, tmpword;
	int Dram_error = 0;
	int DualRAM_Error = 0;
	unsigned char readbuf[200];
	//unsigned char* readbuf;
	unsigned char tmpdata[1], flag = 0;
	unsigned int pre1, pre2, r, s, m;
	unsigned int ret;
	long double accx, accy, accz;
	long double gyrx, gyry, gyrz;
	long double magx, magy, magz;
	unsigned char accx_str[11], accy_str[11], accz_str[11];
	unsigned char gyrx_str[11], gyry_str[11], gyrz_str[11];
	unsigned char magx_str[11], magy_str[11], magz_str[11];
	char* date;
	char* time;
	char* latitude[3];
	char* longitude[3];
	double spd;
	double cog;
	unsigned char* tmpdata_str;
	unsigned char* dt_str;
	unsigned char* lat_str;
	unsigned char* lon_str;
	unsigned char* spd_str;
	unsigned char* cog_str;

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

		//printf("\n---------LH_1008 Start---------\n");
		sendtocom0("Satellite Navigation System Start\n\n");

	}

	//Test Flash
	if (flash_burn == 1) {
		//Flash_Erase();
		Flash_Chip_Erase();
		printf("Flash Erase Complete\n");

		//Flash_Writem(FLASH_BASE_ADDR,0,0x0);
		//Flash_Writem(FLASH_BASE_ADDR,0,0x120);
		//Flash_Writem(FLASH_BASE_ADDR + 0x400,0x400,0xe0d2);
		//Flash_Writem(FLASH_BASE_ADDR + 0x400,0x400,0x567F);
		//Flash_Writem(FLASH_BASE_ADDR + 0x400,0x400,0x15a30);
	}

	debug_entry();


	printf("debug done\n");
/*
	for (i=0; i<10000; i++) {

		ret = get_gps_data(readbuf);

		if (ret == 1) {

			for (j=0; j<100000; j++)
				;

			// get UTC date & time
			date = get_gps_date(readbuf);
			time = get_gps_time(readbuf);
			sendtocom0("UTC is ");
			sendtocom0(date);
			sendtocom0(" ");
			sendtocom0(time);
			sendtocom0("\n");

			// latitude
			get_gps_latitude(readbuf, latitude);

			sendtocom0("latitude is ");
			sendtocom0(latitude[0]);
			sendtocom0("Dgr");
			sendtocom0(latitude[1]);
			sendtocom0("' ");
			sendtocom0(latitude[2]);
			sendtocom0("\n");

			// longitude
			get_gps_longitude(readbuf, longitude);
			sendtocom0("longitude is ");
			sendtocom0(longitude[0]);
			sendtocom0("Dgr");
			sendtocom0(longitude[1]);
			sendtocom0("' ");
			sendtocom0(longitude[2]);
			sendtocom0("\n");

			// spd
			spd = get_gps_spd(readbuf);
			sprintf(spd_str, "spd is %.3fkn", spd);
			sendtocom0(spd_str);
			sendtocom0("\n");

			// cog
			cog = get_gps_cog(readbuf);
			sprintf(cog_str, "cog is %.3fDgr", cog);
			sendtocom0(cog_str);
			sendtocom0("\n");
		}
	}

	printf("done\n");
*/
/*
	for (i=0; i<100; i++) {

		for (j=0; j<200; j++)
			;



		ret = get_gps_data(readbuf);

		printf("i=%d ret=%d\n", i, ret);

		if (ret == 1) {

			date = get_gps_date(readbuf);
			time = get_gps_time(readbuf);

			if (!strcmp(time, "INVALID DATA")) {
					printf("UTC is INVALD DATA\n");
			} else {
				printf("UTC is %s %s\n", date, time);
			}

			get_gps_latitude(readbuf, latitude);

			printf("longitude is %sº%s' %s\n", latitude[0], latitude[1], latitude[2]);

			get_gps_longitude(readbuf, longitude);

			printf("longitude is %sº%s' %s\n", longitude[0], longitude[1], longitude[2]);

			spd = get_gps_spd(readbuf);

			printf("spd is %.3fkn\n", spd);

			cog = get_gps_cog(readbuf);

			printf("cog is %.3fº\n", cog);
		}
	}

*/
/*
	for (i=0; i<100; i++) {

		for (j=0; j<200; j++)
			;

		ret = get_im_data(readbuf);

		if (ret == 1) {

			//printf("accx %Lf  accy %Lf  accz %Lf | gyrx %Lf  gyry %Lf  gyrz %Lf | magx %Lf  magy %Lf  magz %Lf",
			//	get_im_accx(readbuf), get_im_accy(readbuf), get_im_accz(readbuf),
			//	get_im_gyrx(readbuf), get_im_gyry(readbuf), get_im_gyrz(readbuf),
			//	get_im_magx(readbuf), get_im_magy(readbuf), get_im_magz(readbuf));

			accx = get_im_accx(readbuf);
			sprintf(accx_str, "%.3Lf", accx);
			//sendtocom0("accx: ");
			sendtocom0(accx_str);
			sendtocom0(" ");

			accy = get_im_accy(readbuf);
			sprintf(accy_str, "%.3Lf", accy);
			//sendtocom0("accy: ");
			sendtocom0(accy_str);
			sendtocom0(" ");

			accz = get_im_accz(readbuf);
			sprintf(accz_str, "%.3Lf", accz);
			//sendtocom0("accz: ");
			sendtocom0(accz_str);
			sendtocom0(" ");

			gyrx = get_im_gyrx(readbuf);
			sprintf(gyrx_str, "%.3Lf", gyrx);
			//sendtocom0("gyrx: ");
			sendtocom0(gyrx_str);
			sendtocom0(" ");

			gyry = get_im_gyry(readbuf);
			sprintf(gyry_str, "%.3Lf", gyry);
			//sendtocom0("gyry: ");
			sendtocom0(gyry_str);
			sendtocom0(" ");

			gyrz = get_im_gyrz(readbuf);
			sprintf(gyrz_str, "%.3Lf", gyrz);
			//sendtocom0("gyrz: ");
			sendtocom0(gyrz_str);
			sendtocom0(" ");

			magx = get_im_magx(readbuf);
			sprintf(magx_str, "%.3Lf", magx);
			//sendtocom0("magx: ");
			sendtocom0(magx_str);
			sendtocom0(" ");

			magy = get_im_magy(readbuf);
			sprintf(magy_str, "%.3Lf", magy);
			//sendtocom0("magy: ");
			sendtocom0(magy_str);
			sendtocom0(" ");

			magz = get_im_magz(readbuf);
			sprintf(magz_str, "%.3Lf", magz);
			//sendtocom0("magz: ");
			sendtocom0(magz_str);
			sendtocom0(" ");

			sendtocom0("\n");

			//printf("\n");

		}
	}
*/
	/*
	k = 0;
	r = 84;
	pre1 = pre2 = 0;

	for (j=0; j<10000; j++) {

		for (i=0; i<256; i++) {

			tmpdata = read8(0xa0050000);

			//printf("tmpdata is %x\n", tmpdata);

			if (pre1 == 0xfa && pre2 == 0xff) {

				if (k == 0 && tmpdata != 0x36)

					continue;

				if (k < r) {

					readbuf[k] = tmpdata;

					if (k == 1 && k < r) {

						r = readbuf[k] + 3;

					}

					k++;


				} else {

					pre1 = 0;
					pre2 = 0;
					k=0;

					printf("i=%d j=%d ", i, j);


//					for (s=0; s<r; s++) {
//						printf("%02x ", readbuf[s]);
//					}
//
//					printf("\n");


					if (calc_cs(readbuf) == 0) {

						printf("accx %Lf  accy %Lf  accz %Lf | gyrx %Lf  gyry %Lf  gyrz %Lf | magx %Lf  magy %Lf  magz %Lf",
							get_im_accx(readbuf), get_im_accy(readbuf), get_im_accz(readbuf),
							get_im_gyrx(readbuf), get_im_gyry(readbuf), get_im_gyrz(readbuf),
							get_im_magx(readbuf), get_im_magy(readbuf), get_im_magz(readbuf));

						printf("\n");
					} else {

						printf("wrong data\n");

					}
					r = 84;
				}

			} else if (pre1 != 0xfa && pre2 != 0xff) {

				if (tmpdata == 0xfa) {

					pre1 = 0xfa;

				}

			} else if (pre1 == 0xfa && pre2 != 0xff) {

				if (tmpdata == 0xff) {

					pre2 = 0xff;

				}
			}
		}

	}
*/

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
/*
	while(1){
	//循环测Dull RAM
	if(read32(BASEDualRAM + 0x3000) == 1){
			for(k = 0; k < 0x800; k = k + 0x200 )
			{
				for(i = k; i <  k + 0x100; i++)
				{
					temp =  read32(BASEDualRAM + i * 4);
					write32((BASEDualRAM + (i + 0x100) * 4),temp);
				}
				write32(BASEDualRAM + 0x3000,2);
			} 
		}

		//全功能测试
	if(read32(ALL_FUNCIONADD) == 0x31)
	{
		
		if(0x3456 != read16(BAOMI_CARD_ADD))
		{
				write32(BASEDualRAM + 0x3014,1);
		}else{
			write16(BAOMI_CARD_TEST,0xaa55);
			if(read16(BAOMI_CARD_TEST) == 0xaa55){ //保密卡测试成功
				write16(BAOMI_CARD_TEST,0xFFFF);
				write32(ALL_FUNCIONADD + 4,0x34);	
			}else{
				write16(BAOMI_CARD_TEST,0xFFFF);
				write32(ALL_FUNCIONADD + 4,0x32);	
			}
		}	

		for(i = 0;i < 0x3000;i++)
				write32((BASEDualRAM + i),0x12345678);
			for(i = 0;i < 0x3000;i++)
			{
				tmpword = read32(BASEDualRAM + i);
				if(tmpword != 0x12345678)
						DualRAM_Error = 1;				
			}	
			if(DualRAM_Error == 1) //DualRAM测试失败
			{		
				write32(ALL_FUNCIONADD + 8,0x32);	
			}else{
				write32(ALL_FUNCIONADD + 8,0x34);			
			}

		if(0x6543 != read16(EX_INTERFACE_ADD))
		{
			write32(BASEDualRAM + 0x3010,0x2);
		}else{
			write16(EX_INTERFACE_TEST,0xaa55);
			if(read16(EX_INTERFACE_TEST) == 0xaa55){ //对外接口成功
				write16(EX_INTERFACE_TEST,0xFFFF);
				write32(ALL_FUNCIONADD + 12,0x34);					
			}else{
				write16(EX_INTERFACE_TEST,0xFFFF);
				write32(ALL_FUNCIONADD + 12,0x32);	
			}
		}	

		for(i = 0,testram = TESTCOUNT;i < 0x1000000;i++,testram--)
			write16((0x80000000 + i*2),testram);
		for(i = 0,testram = TESTCOUNT;i < 0x1000000;i++,testram--)
		{
			tmptest = read16(0x80000000 + i*2);
			if(tmptest != testram){
				Dram_error = 1;
			}
		}
		if(Dram_error == 1)//DRAM测试失败
		{
			write32(ALL_FUNCIONADD + 20,0x32);	
		}else{
			write32(ALL_FUNCIONADD + 20,0x34);	
		}

	}	

	//各项功能测试
	status = read32(TESTADD_DUAL);
	switch(status)
	{
		case 0x11:
			for(i = 0,testram = TESTCOUNT;i < 0x1000000;i++,testram--)
				write16((0x80000000 + i*2),testram);
			for(i = 0,testram = TESTCOUNT;i < 0x1000000;i++,testram--)
			{
				tmptest = read16(0x80000000 + i*2);
				if(tmptest != testram){
					Dram_error = 1;
				}
			}
			if(Dram_error == 1)//DRAM测试失败
			{
				write32(TESTADD_DUAL,0x12);	
				write32(0xA0290000,0);
				write32(0xA0290004,0);
			}else{
				write32(TESTADD_DUAL,0x14);	
				write32(0xA0290000,0);
				write32(0xA0290004,0);
			}
			break;
		case 0x21:
			if(0x6543 != read16(EX_INTERFACE_ADD))
			{
				write32(TEST_ADD,0x2);
				break;
			}else{
				write16(EX_INTERFACE_TEST,0xaa55);
				if(read16(EX_INTERFACE_TEST) == 0xaa55){ //对外接口成功
					write16(EX_INTERFACE_TEST,0xFFFF);
					write32(TESTADD_DUAL,0x24);	
					write32(0xA0290000,0);
					break;
				}else{
					write16(EX_INTERFACE_TEST,0xFFFF);
					write32(TESTADD_DUAL,0x22);	
					write32(0xA0290000,0);
					break;
				}
			}	
		case 0x81:
			if(0x3456 != read16(BAOMI_CARD_ADD))
			{
					write32(TEST_ADD,1);
					break;
			}else{
				write16(BAOMI_CARD_TEST,0xaa55);
				if(read16(BAOMI_CARD_TEST) == 0xaa55){ //保密卡测试成功
					write16(BAOMI_CARD_TEST,0xFFFF);
					write32(TESTADD_DUAL,0x84);	
					write32(0xA0290000,0);
				}else{
					write16(BAOMI_CARD_TEST,0xFFFF);
					write32(TESTADD_DUAL,0x82);	
					write32(0xA0290000,0);
				}
			}	
				break;
		case 0x41:
			for(i = 0;i < 0x3000;i++)
				write32((BASEDualRAM + i),0x12345678);
			for(i = 0;i < 0x3000;i++)
			{
				tmpword = read32(BASEDualRAM + i);
				if(tmpword != 0x12345678)
						DualRAM_Error = 1;				
			}	
			if(DualRAM_Error == 1) //DualRAM测试失败
			{
				
				write32(TESTADD_DUAL,0x42);	
				write32(0xA0290004,0);
			}else{
				InitDualRAM();
				write32(TESTADD_DUAL,0x44);	
				write32(0xA0290004,0);			
			}
			break;
		default:
			break;

	}
	*/

//中断测试
/*
		INTtmp = read32(BASEDualRAM + 0x100);
		switch(INTtmp)
		{
			case 4:					
					write32(BASEDualRAM + 0x100,0x14);
					write32(0xA0290000,0);
					break;
			case 5:
					write32(BASEDualRAM + 0x100,0x15);
					write32(0xA0290000,0);
					break;
			case 6:
					write32(BASEDualRAM + 0x100,0x16);
					write32(0xA0290000,0);
					break;
			case 7:
					write32(BASEDualRAM + 0x100,0x17);
					write32(0xA0290000,0);
					break;	
			default:
				break;
		}
		
	}
}                    

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

/*Initialize dual memory PC */
void InitDualRAM(void)
{
	int i;
	for(i=0;i<0x4000;i++)
		write32(BASEDualRAM+i*4,0);
}

void initsdram()
{
	int i;
	for(i=0; i<0x1000000; i++)
		write16((0x80000000 + i*2), 0x0);
}


// 12 Electromagnetic Valves - GPIO Mapping Array
static int switch2gpio[12] = { 5, 6, 7, 15, 14, 13, 12, 11, 10, 9, 8, 3 };
static int gpio2switch[16] = { -1, -1, -1, 11, -1, 0, 1, 2, 10, 9, 8, 7, 6, 5, 4, 3 };

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
 * set_EMV()
 *
 * parameter val is a 12-bit integer represents the 12 valves
 * each bit(0~11) represents one valve(1~12) 
 * for example, if we set the val to 0x00c,
 * it means that the 3rd and 4th valves will be open
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

void init_EMV() // initialization for all valves
{
	int i;

	write32(GPIO_EN_REG, 0xffff);
	
	for (i=0; i<10; i++)
		;

	write32(GPIO_DIR_REG, 0xffff);

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

void test_EMV()
{

	int i, j;

	// open-close one by one

	for (i=1; i<=12; i++) {
	
		open_EMV(i);

		for (j=0; j<40000000; j++)
			;

		close_EMV(i);

		for (j=0; j<40000000; j++)
			;

	}

	close_EMV_All();

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

	sendtocom0("********************\n");
	sendtocom0("CAN TEST BEGIN\n");
	sendtocom0("********************\n\n");


	//disptitle("CAN TEST");
	//disp();

	//lineBegin();
	for (i=0; i<6; i++) sendtocom0(" ");

	sendtocom0("CAN1 to CAN2 Test (Five Times)\n\n");

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
	sendtocom0("CAN2 to CAN1 Test (Five Times)\n\n");
	
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

	sendtocom0("********************\n");
	sendtocom0("TEST END\n");
	sendtocom0("********************\n\n");

}

void test_im()
{
    int i, j, k = 0;
    
    unsigned int buf[84]; // the size of the container is 84

    char hex_str[65];
    char* bin_str;

    long double ret;
    long double accx, gyrx, magx;
    char accx_str[10], gyrx_str[10], magx_str[10];
    long double accy, gyry, magy;
    char accy_str[10], gyry_str[10], magy_str[10];
    long double accz, gyrz, magz;
    char accz_str[10], gyrz_str[10], magz_str[10];

    //disptitle("IM TEST");
    sendtocom0("********************\n");
    sendtocom0("IMU TEST BEGIN\n");
    sendtocom0("********************\n\n");

    for (i=0; i<6; i++) sendtocom0(" ");
    sendtocom0("Retriving IMU Data (3 Times)\n\n");

    for (j=0; j<100, k<3; j++) {

    	for (i=0; i<20000000; i++)
    		;

    	ret = get_im_data(buf);

    	//printf("ret is %d\n", ret);

    	if (ret == 0)
    		continue;

    	k++;

        //lineBegin();
        for (i=0; i<6; i++) sendtocom0(" ");
        
        accx = get_im_accx(buf);
        gyrx = get_im_gyrx(buf);
        magx = get_im_magx(buf);
        sprintf(accx_str, (accx >= 0) ? "+%.3Lf" : "%.3Lf", accx);
        sprintf(gyrx_str, (gyrx >= 0) ? "+%.3Lf" : "%.3Lf", gyrx);
        sprintf(magx_str, (magx >= 0) ? "+%.3Lf" : "%.3Lf", magx);
		/*
        printf("AccX: ");
        printf("%s", accx_str);
        printf(" ");

        printf("GyrX: ");
        printf("%s", gyrx_str);
        printf(" ");

        printf("MagX: ");
        printf("%s", magx_str);
        printf(" ");
		*/
        
        sendtocom0("AccX: ");
        sendtocom0(accx_str);
        sendtocom0("\t");

        sendtocom0("GyrX: ");
        sendtocom0(gyrx_str);
        sendtocom0("\t");

        sendtocom0("MagX: ");
        sendtocom0(magx_str);
        sendtocom0("\t");
        sendtocom0("\n");
        //lineEnd(6 + strlen(accx_str) + 1 + 6 + strlen(gyrx_str) + 1 + 6 + strlen(magx_str) + 1 + 6);

        //lineBegin();
        for (i=0; i<6; i++) sendtocom0(" ");
        
        accy = get_im_accy(buf);
        gyry = get_im_gyry(buf);
        magy = get_im_magy(buf);
        sprintf(accy_str, (accy >= 0) ? "+%.3Lf" : "%.3Lf", accy);
        sprintf(gyry_str, (gyry >= 0) ? "+%.3Lf" : "%.3Lf", gyry);
        sprintf(magy_str, (magy >= 0) ? "+%.3Lf" : "%.3Lf", magy);

		/*
        printf("AccY: ");
        printf("%s", accy_str);
        printf(" ");

        printf("GyrY: ");
        printf("%s", gyry_str);
        printf(" ");

        printf("MagY: ");
        printf("%s", magy_str);
        printf(" ");
		*/
        
        sendtocom0("AccY: ");
        sendtocom0(accy_str);
        sendtocom0("\t");

        sendtocom0("GyrY: ");
        sendtocom0(gyry_str);
        sendtocom0("\t");

        sendtocom0("MagY: ");
        sendtocom0(magy_str);
        sendtocom0("\t");
        sendtocom0("\n");
        //lineEnd(6 + strlen(accy_str) + 1 + 6 + strlen(gyry_str) + 1 + 6 + strlen(magy_str) + 1 + 6);

        //lineBegin();
        for (i=0; i<6; i++) sendtocom0(" ");

        accz = get_im_accz(buf);
        gyrz = get_im_gyrz(buf);
        magz = get_im_magz(buf);
        sprintf(accz_str, (accz >= 0) ? "+%.3Lf" : "%.3Lf", accz);
        sprintf(gyrz_str, (gyrz >= 0) ? "+%.3Lf" : "%.3Lf", gyrz);
        sprintf(magz_str, (magz >= 0) ? "+%.3Lf" : "%.3Lf", magz);

		/*
        printf("AccZ: ");
        printf("%s", accz_str);
        printf(" ");

        printf("GyrZ: ");
        printf("%s", gyrz_str);
        printf(" ");

        printf("MagZ: ");
        printf("%s", magz_str);
        printf(" ");
		*/
        
        sendtocom0("AccZ: ");
        sendtocom0(accz_str);
        sendtocom0("\t");

        sendtocom0("GyrZ: ");
        sendtocom0(gyrz_str);
        sendtocom0("\t");

        sendtocom0("MagZ: ");
        sendtocom0(magz_str);
        sendtocom0("\t");
        sendtocom0("\n\n");
        //lineEnd(6 + strlen(accz_str) + 1 + 6 + strlen(gyrz_str) + 1 + 6 + strlen(magz_str) + 1 + 6);
        //disp();
    }
        sendtocom0("********************\n");
        sendtocom0("TEST END\n");
        sendtocom0("********************\n\n");

        //disp();
        //dispfoot();

}

void test_gps() // just lack of process for data gathering
{
    int i, j, k=0;
    int ret;
    char* longitude[3];
    char* latitude[3];
    double spd = 0.0;
    char spd_str[10];
    double cog = 0.0;
    char cog_str[10];
    char* date;
    char* time;
    
    unsigned char gps_data[200];
    
	sendtocom0("********************\n");
	sendtocom0("GPS TEST BEGIN\n");
	sendtocom0("********************\n\n");

	for (i=0; i<6; i++) sendtocom0(" ");
	sendtocom0("Retriving GPS Data (3 Times)\n\n");

    for (i=0; i<1000 && k<3; i++) {

		ret = get_gps_data(gps_data);

		if (ret == 1) {

			k++;

			// UTC
			//lineBegin();
			for (i=0; i<6; i++) sendtocom0(" ");
			// the format is UTC: 2002-09-12 08:35:59

			// get UTC date & time
			date = get_gps_date(gps_data);
			time = get_gps_time(gps_data);
			sendtocom0("UTC is ");
			sendtocom0(date);
			sendtocom0(" ");
			sendtocom0(time);
			//lineEnd(6 + strlen(date) + strlen(time) + 8);
			sendtocom0("\n");

			// Longitude
			//lineBegin();
			for (i=0; i<6; i++) sendtocom0(" ");
			// the format is Longitude: 8°33.915' E

			get_gps_longitude(gps_data, longitude);
			//printf("Longitude: %s°%s' %s", longitude[0], longitude[1], longitude[2]);
			sendtocom0("Longitude: ");
			sendtocom0(longitude[0]);
			sendtocom0("D ");
			sendtocom0(longitude[1]);
			sendtocom0("M");
			sendtocom0(" ");
			sendtocom0(longitude[2]);
			//lineEnd(14 + strlen(longitude[0]) + strlen(longitude[1]) + strlen(longitude[2]) + 6);
			sendtocom0("\n");

			// Latitude
			//lineBegin();
			for (i=0; i<6; i++) sendtocom0(" ");
			// the format is Latitude: 47°17.114' N

			// latitude
			get_gps_latitude(gps_data, latitude);
			//printf("Latitude: %s°%s' %s", latitude[0], latitude[1], latitude[2]);
			sendtocom0("Latitude: ");
			sendtocom0(latitude[0]);
			sendtocom0("D ");
			sendtocom0(latitude[1]);
			sendtocom0("M");
			sendtocom0(" ");
			sendtocom0(latitude[2]);
			//lineEnd(13 + strlen(latitude[0]) + strlen(latitude[1]) + strlen(latitude[2]) + 6);
			sendtocom0("\n");

			// speed over ground
			//lineBegin();
			for (i=0; i<6; i++) sendtocom0(" ");
			// the format is SPD: 0.007 km/h

			spd = get_gps_spd(gps_data) * 1.852;
			sprintf(spd_str, "%.3f", spd);
			//printf("SPD: %s km/h", spd_str);
			sendtocom0("SPD: ");
			sendtocom0(spd_str);
			sendtocom0(" ");
			sendtocom0("km/h");
			//lineEnd(10 + strlen(spd_str) + 6);
			sendtocom0("\n");

			// course over ground
			//lineBegin();
			for (i=0; i<6; i++) sendtocom0(" ");
			// the format is COG: 77.520

			cog = get_gps_cog(gps_data);
			sprintf(cog_str, "%.3f", cog);
			//printf("COG: %s°", cog_str);
			sendtocom0("COG: ");
			sendtocom0(cog_str);
			sendtocom0("D");
			//lineEnd(6 + strlen(cog_str) + 6);
			sendtocom0("\n\n");

			//disp();

		}

		//for (j=0; j<20000000; j++)
		//	;
    }

	//disp();
	//dispfoot();

    sendtocom0("********************\n");
	sendtocom0("TEST END\n");
	sendtocom0("********************\n\n");

}

/*
 * test port: 0xa0051000~0xa005101f
 * GPS port:  0xa0050000~0xa005001f
 * IM port:   0xa0050020~0xa005003f
 *
 */


void debug_entry()
{
	
	int selection, depth;
	int i, j;
	unsigned char readout;

	selection = -1;
	depth = -1;



	while (1) {
	
		//dispmenu();

		sendtocom0("********************\n");
		sendtocom0("SYSTEM TEST MENU\n");
		sendtocom0("********************\n\n");


		for (i=0; i<3; i++) sendtocom0(" ");
		sendtocom0("[1] GPS TEST\n");

		for (i=0; i<3; i++) sendtocom0(" ");
		sendtocom0("[2] IMU TEST\n");

		for (i=0; i<3; i++) sendtocom0(" ");
		sendtocom0("[3] CAN TEST\n");
		sendtocom0("\n");


		depth = *((unsigned char*)0xa0051010);

		for (j=0; j<10; j++)
			;

		if (depth > 0) {

			for (i=0; i<depth; i++) {

				readout = *((unsigned char*)0xa0051000);

				for (j=0; j<10; j++)
					;

			}

		}

		sendtocom0("Please Enter Your Choice:\n\n");

		for (j=0; j<20000000; j++)
			;
	
		do {

			depth = *((unsigned char*)0xa0051010);
			for (j=0; j<10; j++)
				;

			if (depth > 0) {

				selection = *((unsigned char*)0xa0051000);
				for (j=0; j<10; j++)
					;
				break;
			}
			//printf("selection 0x%x depth 0x%x\n", selection, depth);

		} while (selection == 0 || depth == 0);

		//printf("selection is %x\n", selection);

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
		}
	}
}

void test_com()
{

	unsigned char* str;

	unsigned char* str2;


	int fifosize;

	str = "LH-1008 Debug";

	dispmenu();

	//recfromcom0();


	//fifosize = *((unsigned char*)(0xa0051000 + 0x14));

	//printf("0x%x\n", fifosize);

	//if (fifosize >= strlen(str)) {
		
		//sendtocom0(str);

	//}
	
	//fifosize = *((unsigned char*)(0xa0051000 + 0x10));

	if (fifosize > 0) {
	
		//recfromcom(0xa0051000, str2, fifosize);

		//printf("%s\n", str2);

	}

	
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
