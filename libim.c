#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "LH_1008.h"
#include "libim.h"

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

int calc_im_cs(int buf[])
{

	int i;

	int sum = 0xFF;

	for (i=0; i<84; i++) {

		sum += buf[i];

	}


	if ((sum & 0x00ff) == 0x0) {

		return 0;

	} else {

		return -1;
	}

}

int get_im_data(unsigned int readbuf[])
{
	int j, s;
	int k = 0;
	int r = 84;
	int pre1 = 0;
	int pre2 = 0;
	unsigned char tmpdata[1];
	int cnt;
	char* cnt_str;

	unsigned char ret;

	for (j=0; j<5000; j++) {

		//for (i=0; i<256 && flag == 0; i++) {

			//tmpdata = read8(0xa0050000);

			ret = recfromcom(0xa0050000, tmpdata);

			if (ret == 0)
				continue;
			//sprintf(tmpdata_str, "%c", tmpdata);

			//sendtocom0(tmpdata_str);

			//for (s=0; s<20000; s++)
			//	;

			if (pre1 == 0xfa && pre2 == 0xff) {

				if (k < r) {

					readbuf[k++] = tmpdata[0];

					if (k == 1 && readbuf[0] != 0x36) {

						//j = 0;
						pre1 = 0;
						pre2 = 0;
						k = 0;

					}

					//k++;

				} else {

					if (calc_im_cs(readbuf) == 0) {

						return 1;

					}

						//j = 0;
						pre1 = 0;
						pre2 = 0;
						k=0;


				}

			} else if (pre1 != 0xfa && pre2 != 0xff) {

				if (tmpdata[0] == 0xfa) {

					pre1 = 0xfa;

				}

			} else if (pre1 == 0xfa && pre2 != 0xff) {

				if (tmpdata[0] == 0xff) {

					pre2 = 0xff;

				} else {

					pre1 = 0;
					pre2 = 0;
					k = 0;
				}
			}
		//}
	}

	return 0;
}
//FA FF MTData2 51 {
//(Acceleration|Double, 24 bytes, (accX:  -1.26404447, accY:   0.03770998, accZ:  65.35904471)),
//(RateOfTurn|Double, 24 bytes, (gyrX:   6.34589114, gyrY:  -0.03657224, gyrZ:  -0.01993106)),
//(MagneticField|Double, 24 bytes, (magX:  -0.76577867, magY:   0.29737438, magZ:  -1.39175252))
//}

//FA FF 36 51
//40 23 18
//BF F3 90 7C A0 00 00 00
//3F F7 5F 9C 00 00 00 00
//40 4D 96 99 E0 00 00 00
//80 23 18
//40 17 AF 4D A0 00 00 01
//BF A1 77 7A 40 00 00 00
//BF 94 08 89 40 00 00 01
//C0 23 18
//BF B6 77 6A 40 00 00 00
//BF AF 71 70 A0 00 00 00
//BF F5 7B CB E0 00 00 00

/*
int main()
{
	int buf[84]; // the size of the container is 84
	char hex_str[65];
	char* bin_str;
	int i;
	long double ret;

	buf[0] = 0x36;
	buf[1] = 0x51; // length
	buf[2] = 0x40; // acc
	buf[3] = 0x23;
	buf[4] = 0x18; // length
	buf[5] = 0xBF; // x
	buf[6] = 0xF3;
	buf[7] = 0x90;
	buf[8] = 0x7C;
	buf[9] = 0xA0;
	buf[10] = 0x00;
	buf[11] = 0x00;
	buf[12] = 0x00;
	buf[13] = 0x3F; // y
	buf[14] = 0xF7;
	buf[15] = 0x5F;
	buf[16] = 0x9C;
	buf[17] = 0x00;
	buf[18] = 0x00;
	buf[19] = 0x00;
	buf[20] = 0x00;
	buf[21] = 0x40; // z
	buf[22] = 0x4D;
	buf[23] = 0x96;
	buf[24] = 0x99;
	buf[25] = 0xE0;
	buf[26] = 0x00;
	buf[27] = 0x00;
	buf[28] = 0x00;
	buf[29] = 0x80; // gyr
	buf[30] = 0x23;
	buf[31] = 0x18; // length
	buf[32] = 0x40; // x
	buf[33] = 0x17;
	buf[34] = 0xAF;
	buf[35] = 0x4D;
	buf[36] = 0xA0;
	buf[37] = 0x00;
	buf[38] = 0x00;
	buf[39] = 0x01;
	buf[40] = 0xBF; // y
	buf[41] = 0xA1;
	buf[42] = 0x77;
	buf[43] = 0x7A;
	buf[44] = 0x40;
	buf[45] = 0x00;
	buf[46] = 0x00;
	buf[47] = 0x00;
	buf[48] = 0xBF; // z
	buf[49] = 0x94;
	buf[50] = 0x08;
	buf[51] = 0x89;
	buf[52] = 0x40;
	buf[53] = 0x00;
	buf[54] = 0x00;
	buf[55] = 0x01;
	buf[56] = 0xC0; // mag
	buf[57] = 0x23;
	buf[58] = 0x18; // length
	buf[59] = 0xBF; // x
	buf[60] = 0xB6;
	buf[61] = 0x77;
	buf[62] = 0x6A;
	buf[63] = 0x40;
	buf[64] = 0x00;
	buf[65] = 0x00;
	buf[66] = 0x00;
	buf[67] = 0xBF; // y
	buf[68] = 0xAF;
	buf[69] = 0x71;
	buf[70] = 0x70;
	buf[71] = 0xA0;
	buf[72] = 0x00;
	buf[73] = 0x00;
	buf[74] = 0x00;
	buf[75] = 0xBF; // z
	buf[76] = 0xF5;
	buf[77] = 0x7B;
	buf[78] = 0xCB;
	buf[79] = 0xE0;
	buf[80] = 0x00;
	buf[81] = 0x00;
	buf[82] = 0x00;
	buf[83] = 0x15; // checksum

	calc_im_cs(buf);

	ret = get_im_accx(buf);
	printf("accx is %Lf\n", ret);

	ret = get_im_accy(buf);
	printf("accy is %Lf\n", ret);

	ret = get_im_accz(buf);
	printf("accz is %Lf\n", ret);

	ret = get_im_gyrx(buf);
	printf("gyrx is %Lf\n", ret);

	ret = get_im_gyry(buf);
	printf("gyry is %Lf\n", ret);

	ret = get_im_gyrz(buf);
	printf("gyrz is %Lf\n", ret);

	ret = get_im_magx(buf);
	printf("magx is %Lf\n", ret);


	ret = get_im_magy(buf);
	printf("magy is %Lf\n", ret);

	ret = get_im_magz(buf);
	printf("magz is %Lf\n", ret);

}
*/

long double get_im_accx(int* im_data)
{

	char hex_str[65];
	char* bin_str;
	long double ret;

	long long accx_data = (((long long)im_data[5]) << 8*7) +
							(((long long)im_data[6]) << 8*6) +
							(((long long)im_data[7]) << 8*5) +
							(((long long)im_data[8]) << 8*4) +
							(((long long)im_data[9]) << 8*3) +
							(((long long)im_data[10]) << 8*2) +
							(((long long)im_data[11]) << 8*1) +
							((long long)im_data[12]);

	sprintf(hex_str, "%llx", accx_data);
	bin_str = hex_to_bin(hex_str);
	ret = ieee_to_decimal(bin_str);
	return ret;
}

long double get_im_accy(int* im_data)
{

	char hex_str[65];
	char* bin_str;
	long double ret;

	long long accy_data = (((long long)im_data[13]) << 8*7) +
							(((long long)im_data[14]) << 8*6) +
							(((long long)im_data[15]) << 8*5) +
							(((long long)im_data[16]) << 8*4) +
							(((long long)im_data[17]) << 8*3) +
							(((long long)im_data[18]) << 8*2) +
							(((long long)im_data[19]) << 8*1) +
							((long long)im_data[20]);

	sprintf(hex_str, "%llx", accy_data);
	bin_str = hex_to_bin(hex_str);
	ret = ieee_to_decimal(bin_str);
	return ret;
}

long double get_im_accz(int* im_data)
{

	char hex_str[65];
	char* bin_str;
	long double ret;

	long long accz_data = (((long long)im_data[21]) << 8*7) +
							(((long long)im_data[22]) << 8*6) +
							(((long long)im_data[23]) << 8*5) +
							(((long long)im_data[24]) << 8*4) +
							(((long long)im_data[25]) << 8*3) +
							(((long long)im_data[26]) << 8*2) +
							(((long long)im_data[27]) << 8*1) +
							((long long)im_data[28]);

	sprintf(hex_str, "%llx", accz_data);
	bin_str = hex_to_bin(hex_str);
	ret = ieee_to_decimal(bin_str);
	return ret;
}

long double get_im_gyrx(int* im_data)
{

	char hex_str[65];
	char* bin_str;
	long double ret;

	long long gyrx_data = (((long long)im_data[32]) << 8*7) +
							(((long long)im_data[33]) << 8*6) +
							(((long long)im_data[34]) << 8*5) +
							(((long long)im_data[35]) << 8*4) +
							(((long long)im_data[36]) << 8*3) +
							(((long long)im_data[37]) << 8*2) +
							(((long long)im_data[38]) << 8*1) +
							((long long)im_data[39]);

	sprintf(hex_str, "%llx", gyrx_data);
	bin_str = hex_to_bin(hex_str);
	ret = ieee_to_decimal(bin_str);
	return ret;
}

long double get_im_gyry(int* im_data)
{

	char hex_str[65];
	char* bin_str;
	long double ret;

	long long gyry_data = (((long long)im_data[40]) << 8*7) +
							(((long long)im_data[41]) << 8*6) +
							(((long long)im_data[42]) << 8*5) +
							(((long long)im_data[43]) << 8*4) +
							(((long long)im_data[44]) << 8*3) +
							(((long long)im_data[45]) << 8*2) +
							(((long long)im_data[46]) << 8*1) +
							((long long)im_data[47]);

	sprintf(hex_str, "%llx", gyry_data);
	bin_str = hex_to_bin(hex_str);
	ret = ieee_to_decimal(bin_str);
	return ret;
}

long double get_im_gyrz(int* im_data)
{

	char hex_str[65];
	char* bin_str;
	long double ret;

	long long gyrz_data = (((long long)im_data[48]) << 8*7) +
							(((long long)im_data[49]) << 8*6) +
							(((long long)im_data[50]) << 8*5) +
							(((long long)im_data[51]) << 8*4) +
							(((long long)im_data[52]) << 8*3) +
							(((long long)im_data[53]) << 8*2) +
							(((long long)im_data[54]) << 8*1) +
							((long long)im_data[55]);

	sprintf(hex_str, "%llx", gyrz_data);
	bin_str = hex_to_bin(hex_str);
	ret = ieee_to_decimal(bin_str);
	return ret;
}

long double get_im_magx(int* im_data)
{

	char hex_str[65];
	char* bin_str;
	long double ret;

	long long magx_data = (((long long)im_data[59]) << 8*7) +
							(((long long)im_data[60]) << 8*6) +
							(((long long)im_data[61]) << 8*5) +
							(((long long)im_data[62]) << 8*4) +
							(((long long)im_data[63]) << 8*3) +
							(((long long)im_data[64]) << 8*2) +
							(((long long)im_data[65]) << 8*1) +
							((long long)im_data[66]);

	sprintf(hex_str, "%llx", magx_data);
	bin_str = hex_to_bin(hex_str);
	ret = ieee_to_decimal(bin_str);
	return ret;
}

long double get_im_magy(int* im_data)
{

	char hex_str[65];
	char* bin_str;
	long double ret;

	long long magy_data = (((long long)im_data[67]) << 8*7) +
							(((long long)im_data[68]) << 8*6) +
							(((long long)im_data[69]) << 8*5) +
							(((long long)im_data[70]) << 8*4) +
							(((long long)im_data[71]) << 8*3) +
							(((long long)im_data[72]) << 8*2) +
							(((long long)im_data[73]) << 8*1) +
							((long long)im_data[74]);

	sprintf(hex_str, "%llx", magy_data);
	bin_str = hex_to_bin(hex_str);
	ret = ieee_to_decimal(bin_str);
	return ret;
}

long double get_im_magz(int* im_data)
{

	char hex_str[65];
	char* bin_str;
	long double ret;

	long long magz_data = (((long long)im_data[75]) << 8*7) +
							(((long long)im_data[76]) << 8*6) +
							(((long long)im_data[77]) << 8*5) +
							(((long long)im_data[78]) << 8*4) +
							(((long long)im_data[79]) << 8*3) +
							(((long long)im_data[80]) << 8*2) +
							(((long long)im_data[81]) << 8*1) +
							((long long)im_data[82]);

	sprintf(hex_str, "%llx", magz_data);
	bin_str = hex_to_bin(hex_str);
	ret = ieee_to_decimal(bin_str);
	return ret;
}

/*
long double get_im_accx(int* im_data)
{

	char hex_str[65];
	char* bin_str;
	long double ret;

	long long accx_data = (((long long)im_data[58]) << 8*7) +
							(((long long)im_data[59]) << 8*6) +
							(((long long)im_data[60]) << 8*5) +
							(((long long)im_data[61]) << 8*4) +
							(((long long)im_data[62]) << 8*3) +
							(((long long)im_data[63]) << 8*2) +
							(((long long)im_data[64]) << 8*1) +
							((long long)im_data[65]);

	sprintf(hex_str, "%llx", accx_data);
	bin_str = hex_to_bin(hex_str);
	ret = ieee_to_decimal(bin_str);
	return ret;
}

long double get_im_accy(int* im_data)
{

	char hex_str[65];
	char* bin_str;
	long double ret;

	long long accy_data = (((long long)im_data[66]) << 8*7) +
							(((long long)im_data[67]) << 8*6) +
							(((long long)im_data[68]) << 8*5) +
							(((long long)im_data[69]) << 8*4) +
							(((long long)im_data[70]) << 8*3) +
							(((long long)im_data[71]) << 8*2) +
							(((long long)im_data[72]) << 8*1) +
							((long long)im_data[73]);

	sprintf(hex_str, "%llx", accy_data);
	bin_str = hex_to_bin(hex_str);
	ret = ieee_to_decimal(bin_str);
	return ret;
}

long double get_im_accz(int* im_data)
{

	char hex_str[65];
	char* bin_str;
	long double ret;

	long long accz_data = (((long long)im_data[74]) << 8*7) +
							(((long long)im_data[75]) << 8*6) +
							(((long long)im_data[76]) << 8*5) +
							(((long long)im_data[77]) << 8*4) +
							(((long long)im_data[78]) << 8*3) +
							(((long long)im_data[79]) << 8*2) +
							(((long long)im_data[80]) << 8*1) +
							((long long)im_data[81]);

	sprintf(hex_str, "%llx", accz_data);
	bin_str = hex_to_bin(hex_str);
	ret = ieee_to_decimal(bin_str);
	return ret;
}

long double get_im_gyrx(int* im_data)
{

	char hex_str[65];
	char* bin_str;
	long double ret;

	long long gyrx_data = (((long long)im_data[85]) << 8*7) +
							(((long long)im_data[86]) << 8*6) +
							(((long long)im_data[87]) << 8*5) +
							(((long long)im_data[88]) << 8*4) +
							(((long long)im_data[89]) << 8*3) +
							(((long long)im_data[90]) << 8*2) +
							(((long long)im_data[91]) << 8*1) +
							((long long)im_data[92]);

	sprintf(hex_str, "%llx", gyrx_data);
	bin_str = hex_to_bin(hex_str);
	ret = ieee_to_decimal(bin_str);
	return ret;
}

long double get_im_gyry(int* im_data)
{

	char hex_str[65];
	char* bin_str;
	long double ret;

	long long gyry_data = (((long long)im_data[93]) << 8*7) +
							(((long long)im_data[94]) << 8*6) +
							(((long long)im_data[95]) << 8*5) +
							(((long long)im_data[96]) << 8*4) +
							(((long long)im_data[97]) << 8*3) +
							(((long long)im_data[98]) << 8*2) +
							(((long long)im_data[99]) << 8*1) +
							((long long)im_data[100]);

	sprintf(hex_str, "%llx", gyry_data);
	bin_str = hex_to_bin(hex_str);
	ret = ieee_to_decimal(bin_str);
	return ret;
}

long double get_im_gyrz(int* im_data)
{

	char hex_str[65];
	char* bin_str;
	long double ret;

	long long gyrz_data = (((long long)im_data[101]) << 8*7) +
							(((long long)im_data[102]) << 8*6) +
							(((long long)im_data[103]) << 8*5) +
							(((long long)im_data[104]) << 8*4) +
							(((long long)im_data[105]) << 8*3) +
							(((long long)im_data[106]) << 8*2) +
							(((long long)im_data[107]) << 8*1) +
							((long long)im_data[108]);

	sprintf(hex_str, "%llx", gyrz_data);
	bin_str = hex_to_bin(hex_str);
	ret = ieee_to_decimal(bin_str);
	return ret;
}

long double get_im_magx(int* im_data)
{

	char hex_str[65];
	char* bin_str;
	long double ret;

	long long magx_data = (((long long)im_data[112]) << 8*7) +
							(((long long)im_data[113]) << 8*6) +
							(((long long)im_data[114]) << 8*5) +
							(((long long)im_data[115]) << 8*4) +
							(((long long)im_data[116]) << 8*3) +
							(((long long)im_data[117]) << 8*2) +
							(((long long)im_data[118]) << 8*1) +
							((long long)im_data[119]);

	sprintf(hex_str, "%llx", magx_data);
	bin_str = hex_to_bin(hex_str);
	ret = ieee_to_decimal(bin_str);
	return ret;
}

long double get_im_magy(int* im_data)
{

	char hex_str[65];
	char* bin_str;
	long double ret;

	long long magy_data = (((long long)im_data[120]) << 8*7) +
							(((long long)im_data[121]) << 8*6) +
							(((long long)im_data[122]) << 8*5) +
							(((long long)im_data[123]) << 8*4) +
							(((long long)im_data[124]) << 8*3) +
							(((long long)im_data[125]) << 8*2) +
							(((long long)im_data[126]) << 8*1) +
							((long long)im_data[127]);

	sprintf(hex_str, "%llx", magy_data);
	bin_str = hex_to_bin(hex_str);
	ret = ieee_to_decimal(bin_str);
	return ret;
}

long double get_im_magz(int* im_data)
{

	char hex_str[65];
	char* bin_str;
	long double ret;

	long long magz_data = (((long long)im_data[128]) << 8*7) +
							(((long long)im_data[129]) << 8*6) +
							(((long long)im_data[130]) << 8*5) +
							(((long long)im_data[131]) << 8*4) +
							(((long long)im_data[132]) << 8*3) +
							(((long long)im_data[133]) << 8*2) +
							(((long long)im_data[134]) << 8*1) +
							((long long)im_data[135]);

	sprintf(hex_str, "%llx", magz_data);
	bin_str = hex_to_bin(hex_str);
	ret = ieee_to_decimal(bin_str);
	return ret;
}
*/
long double ieee_to_decimal(char* bin_str)
{

    int sign = 1;
    int power = -1;
    int exp = 0;
    int i = 11;
    long double total = 0.0;
    long double value = 0.0;
	int k = 0;

	char bin_str_cpy[65];

	strncpy(bin_str_cpy, bin_str, strlen(bin_str));

	bin_str_cpy[strlen(bin_str)] = '\0';


    if (bin_str_cpy[0] == '1')
    	sign = -sign;

    while (i >= 1) {

        if (bin_str_cpy[i] == '1') {
            exp = exp + pow(2, 11-i);
        }

        i--;

    }

    exp -= 1023;

    for (k=0; k<52; k++) {

    	if (bin_str_cpy[k + 12] == '1') {

    		total += (double)pow(2.0, power);

    	}

    	power--;

    }

    total += 1.0;

    value = sign * pow(2.0, exp) * total;

    return value;
}

char* hex_to_bin(char* hex_str)
{

	static char bin_str[65];
	int i, j;
	for (i=0, j=0; i<16; i++, j+=4) {

		switch (hex_str[i]) {
		case '0':
			bin_str[j] = '0';
			bin_str[j+1] = '0';
			bin_str[j+2] = '0';
			bin_str[j+3] = '0';
			break;
		case '1':
			bin_str[j] = '0';
			bin_str[j+1] = '0';
			bin_str[j+2] = '0';
			bin_str[j+3] = '1';
			break;
		case '2':
			bin_str[j] = '0';
			bin_str[j+1] = '0';
			bin_str[j+2] = '1';
			bin_str[j+3] = '0';
			break;
		case '3':
			bin_str[j] = '0';
			bin_str[j+1] = '0';
			bin_str[j+2] = '1';
			bin_str[j+3] = '1';
			break;
		case '4':
			bin_str[j] = '0';
			bin_str[j+1] = '1';
			bin_str[j+2] = '0';
			bin_str[j+3] = '0';
			break;
		case '5':
			bin_str[j] = '0';
			bin_str[j+1] = '1';
			bin_str[j+2] = '0';
			bin_str[j+3] = '1';
			break;
		case '6':
			bin_str[j] = '0';
			bin_str[j+1] = '1';
			bin_str[j+2] = '1';
			bin_str[j+3] = '0';
			break;
		case '7':
			bin_str[j] = '0';
			bin_str[j+1] = '1';
			bin_str[j+2] = '1';
			bin_str[j+3] = '1';
			break;
		case '8':
			bin_str[j] = '1';
			bin_str[j+1] = '0';
			bin_str[j+2] = '0';
			bin_str[j+3] = '0';
			break;
		case '9':
			bin_str[j] = '1';
			bin_str[j+1] = '0';
			bin_str[j+2] = '0';
			bin_str[j+3] = '1';
			break;
		case 'a':
			bin_str[j] = '1';
			bin_str[j+1] = '0';
			bin_str[j+2] = '1';
			bin_str[j+3] = '0';
			break;
		case 'b':
			bin_str[j] = '1';
			bin_str[j+1] = '0';
			bin_str[j+2] = '1';
			bin_str[j+3] = '1';
			break;
		case 'c':
			bin_str[j] = '1';
			bin_str[j+1] = '1';
			bin_str[j+2] = '0';
			bin_str[j+3] = '0';
			break;
		case 'd':
			bin_str[j] = '1';
			bin_str[j+1] = '1';
			bin_str[j+2] = '0';
			bin_str[j+3] = '1';
			break;
		case 'e':
			bin_str[j] = '1';
			bin_str[j+1] = '1';
			bin_str[j+2] = '1';
			bin_str[j+3] = '0';
			break;
		case 'f':
			bin_str[j] = '1';
			bin_str[j+1] = '1';
			bin_str[j+2] = '1';
			bin_str[j+3] = '1';
			break;
		default:
			break;
		}
	}

	bin_str[64] = '\0';

	return bin_str;
}

void test_im()
{
    int i, j, k = 0;

    unsigned int buf[84]; // the size of the container is 84

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


