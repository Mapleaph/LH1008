#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int get_gps_data(unsigned char readbuf[]);
char* get_gps_date(char* gps_data);
char* get_gps_time(char* gps_data);
void get_gps_latitude(char* gps_data, char* latitude[]);
void get_gps_longitude(char* gps_data, char* longitude[]);
double get_gps_spd(char* gps_data);
double get_gps_cog(char* gps_data);

int get_dec_part(double num);
double get_frac_part(double num);

/*
int main()
{
	char* date;
	char* time;
	char* latitude[3];
	char* longitude[3];
	double spd;
	double cog;
	char* p;

	char gps_data[] = "$GPRMC,083559.00,A,4717.11437,N,00833.91522,E,0.004,77.52,091202,,,A*57";

	date = get_gps_date(gps_data);

	//printf("date is %s\n", date);

	time = get_gps_time(gps_data);

	//printf("time is %s\n", time);

	if (!strcmp(time, "INVALID DATA")) {
		printf("UTC is INVALD DATA\n");
	} else {
		printf("UTC is %s %s\n", date, time);	
	}

	get_gps_latitude(gps_data, latitude);

	printf("longitude is %sº%s' %s\n", latitude[0], latitude[1], latitude[2]);

	get_gps_longitude(gps_data, longitude);

	printf("longitude is %sº%s' %s\n", longitude[0], longitude[1], longitude[2]);

	spd = get_gps_spd(gps_data);

	printf("spd is %.3fkn\n", spd);

	cog = get_gps_cog(gps_data);

	printf("cog is %.3fº\n", cog);
	
	return 0;
}
*/

/*
char* get_gps_data()
{
	
	int i, j;

	int fifosize;

	unsigned char* buf;

	unsigned char chr;

	// starts with $ and ends with newline

	fifosize = *((unsigned char*)(0xa0050000 + 0x10));
	
	for (j=0; j<10; j++)
		;

	if (fifosize > 0) {
		
		chr = *((unsigned char*)(0xa0050000));

		for (j=0; j<10; j++)
			;

		if (chr == '$') {
			
			*buf = chr;

			do {
			
				chr = *((unsigned char*)(0xa0050000));

				for (j=0; j<10; j++)
					;

				*(buf+1) = chr;

			} while (chr != '\n');

			return buf;
		}

	}
}
*/

int calc_gps_cs(char readbuf[], unsigned int length)
{

	int result = readbuf[0];

	char result_str[3];

	char checksum[3];

	int i;

	for (i=1; readbuf[i]!='*'; i++) {

		result = result ^ readbuf[i];

	}

	sprintf(result_str, "%x", result);

	checksum[0] = readbuf[length - 2];

	checksum[1] = readbuf[length - 1];

	checksum[2] = '\0';

	if (strcmp(result_str, checksum) == 0) {

		return 0;

	} else {

		return -1;

	}

}

char* get_gps_date(char* gps_data)
{
	static char ret[11]; // if not static, tmp returned will be destroyed when the function ends.
	char* tmp[11];
	char* date;
	char* p;
	int i = 0;
	int k=0;
	
	char gps_data_cpy[200];

	//
	// if we don't make a copy of the original gps_data,
	// then, the gps_data will be changed during the strtok function invocation.
	//
	strncpy(gps_data_cpy, gps_data, strlen(gps_data));

	p = strtok(gps_data_cpy, ",");

	while (p != NULL) {

		k++;

		tmp[i++] = p;

		p = strtok(NULL, ",");

	}

	if (strcmp(tmp[0], "GPRMC")) {

		return "INVALID DATA";
	}

	if (k == 11) {

		date = tmp[9];

		ret[0] = '2';
		ret[1] = '0';
		ret[2] = *(date+4);
		ret[3] = *(date+5);
		ret[4] = '-';
		ret[5] = *(date+2);
		ret[6] = *(date+3);
		ret[7] = '-';
		ret[8] = *(date);
		ret[9] = *(date+1);
		ret[10] = '\0';

	} else {

		ret[0] = '?';
		ret[1] = '?';
		ret[2] = '?';
		ret[3] = '?';
		ret[4] = '-';
		ret[5] = '?';
		ret[6] = '?';
		ret[7] = '-';
		ret[8] = '?';
		ret[9] = '?';
		ret[10] = '\0';

	}

	return ret;
}

char* get_gps_time(char* gps_data)
{
	static char ret[13]; // if not static, tmp returned will be destroyed when the function ends.
	char* tmp[11];
	char* time;
	char* p;
	int i = 0;
	int k = 0;

	char gps_data_cpy[200];

	//
	// if we don't make a copy of the original gps_data,
	// then, the gps_data will be changed during the strtok function invocation.
	//
	strncpy(gps_data_cpy, gps_data, strlen(gps_data));

	p = strtok(gps_data_cpy, ",");

	while (p != NULL) {

		k++;

		tmp[i++] = p;

		p = strtok(NULL, ",");

	}

	if (strcmp(tmp[0], "GPRMC")) {

		return "INVALID DATA";
	}


	if (k == 11) {

		time = tmp[1];

		ret[0] = *time;
		ret[1] = *(time+1);
		ret[2] = ':';
		ret[3] = *(time+2);
		ret[4] = *(time+3);
		ret[5] = ':';
		ret[6] = *(time+4);
		ret[7] = *(time+5);
		ret[8] = *(time+6);
		ret[9] = ((time+7) != NULL) ? *(time+7) : '0';
		ret[10] = ((time+8) != NULL) ? *(time+8) : '0';
		ret[11] = ((time+8) != NULL) ? *(time+8) : '0';
		ret[12] = '\0';

	} else {

		ret[0] = '?';
		ret[1] = '?';
		ret[2] = ':';
		ret[3] = '?';
		ret[4] = '?';
		ret[5] = ':';
		ret[6] = '?';
		ret[7] = '?';
		ret[8] = '.';
		ret[9] = '?';
		ret[10] = '?';
		ret[11] = '?';
		ret[12] = '\0';


	}

	return ret;
}

void get_gps_latitude(char* gps_data, char* latitude[])
{
	char* tmp[11];
	char* buf;
	char* p;
	char* indicator;

	double num;
	int dec_part;
	double frac_part;
	int i = 0;
	int k = 0;

	char degree_str[10];
	char minute_str[10];
	char indicator_str[10];

	char gps_data_cpy[200];

	//
	// if we don't make a copy of the original gps_data,
	// then, the gps_data will be changed during the strtok function invocation.
	//
	strncpy(gps_data_cpy, gps_data, strlen(gps_data));

	p = strtok(gps_data_cpy, ",");

	while (p != NULL) {

		k++;

		tmp[i++] = p;

		p = strtok(NULL, ",");

	}

	
	if (strcmp(tmp[0], "GPRMC")) {

		latitude[0] = "INVALID DATA";

	}

	if (k == 11) {

		buf = tmp[3];

		indicator = tmp[4];

		num = atof(buf) / 100;

		dec_part = get_dec_part(num);

		frac_part = get_frac_part(num);

		sprintf(degree_str, "%d", dec_part);
		sprintf(minute_str, "%.3f", frac_part * 100);
		sprintf(indicator_str, "%s", indicator);

		latitude[0] = degree_str;
		latitude[1] = minute_str;
		latitude[2] = indicator_str;

	} else {

		latitude[0] = "??";
		latitude[1] = "???";
		latitude[2] = "?";

	}

}

void get_gps_longitude(char* gps_data, char* longitude[])
{

	char* tmp[11];
	char* buf;
	char* p;
	char* indicator;

	double num;
	int dec_part;
	double frac_part;
	int i = 0;
	int k = 0;

	char degree_str[10];
	char minute_str[10];
	char indicator_str[10];

	char gps_data_cpy[200];

	//
	// if we don't make a copy of the original gps_data,
	// then, the gps_data will be changed during the strtok function invocation.
	//
	strncpy(gps_data_cpy, gps_data, strlen(gps_data));

	p = strtok(gps_data_cpy, ",");



	while (p != NULL) {

		k++;

		tmp[i++] = p;

		p = strtok(NULL, ",");

	}

	
	if (strcmp(tmp[0], "GPRMC")) {

		longitude[0] = "INVALID DATA";

	}

	if (k == 11) {

		buf = tmp[5];
		indicator = tmp[6];

		num = atof(buf) / 100;

		dec_part = get_dec_part(num);

		frac_part = get_frac_part(num);

		sprintf(degree_str, "%d", dec_part);
		sprintf(minute_str, "%.3f", frac_part * 100);
		sprintf(indicator_str, "%s", indicator);

		longitude[0] = degree_str;
		longitude[1] = minute_str;
		longitude[2] = indicator_str;

	} else {

		longitude[0] = "??";
		longitude[1] = "???";
		longitude[2] = "?";
	}

}

double get_gps_spd(char* gps_data)
{
	char* tmp[11];
	char* p;
	double spd;
	int i = 0;
	int k = 0;

	char gps_data_cpy[200];

	//
	// if we don't make a copy of the original gps_data,
	// then, the gps_data will be changed during the strtok function invocation.
	//
	strncpy(gps_data_cpy, gps_data, strlen(gps_data));

	p = strtok(gps_data_cpy, ",");

	while (p != NULL) {

		k++;

		tmp[i++] = p;

		p = strtok(NULL, ",");

	}

	if (strcmp(tmp[0], "GPRMC")) {

		return -1;
	}


	if (k == 11) {

		spd = atof(tmp[7]);

	} else {

		spd = 0.0f;

	}

	return spd;

}

double get_gps_cog(char* gps_data)
{
	char* tmp[11];
	char* p;
	double cog;
	int i = 0;
	int k = 0;

	char gps_data_cpy[200];

	//
	// if we don't make a copy of the original gps_data,
	// then, the gps_data will be changed during the strtok function invocation.
	//
	strncpy(gps_data_cpy, gps_data, strlen(gps_data));

	p = strtok(gps_data_cpy, ",");

	while (p != NULL) {

		k++;

		tmp[i++] = p;

		p = strtok(NULL, ",");

	}

	if (strcmp(tmp[0], "GPRMC")) {

		return -1;
	}

	if (k == 11) {

		cog = atof(tmp[8]);

	} else {

		cog = 0.0f;

	}
	
	return cog;
}

int get_dec_part(double num)
{

	int dec_part = (int)num;

	if (dec_part > num) {
		dec_part -= 1;
	}

	return dec_part;

}

double get_frac_part(double num)
{

	int dec_part = get_dec_part(num);

	return num - dec_part;

}

int get_gps_data(unsigned char readbuf[])
{
	int i, j, s=0;
	int k = 0;
	int pre1 = 0;
	unsigned char tmpdata[1];
	unsigned int ret;
	unsigned char* tmpdata_str;
	char gps_data_cpy[200];
	char* p;


	for (j=0; j<5000 && k<256; j++) {

		//for (i=0; i<256 && flag == 0; i++) {

			//tmpdata = read8(0xa0050000);

			for (i=0; i<20000; i++)
				;

			ret = recfromcom(0xa0050000, tmpdata);

			if (ret == 0)
				continue;

			//sprintf(tmpdata_str, "%c", tmpdata[0]);

			//sendtocom0(tmpdata_str);

			//for (s=0; s<100; s++)
			//	;

			if (pre1 == '$') {

				readbuf[k] = tmpdata[0];

				if (k > 2) {

					if ((readbuf[k-1] == '\r') && (readbuf[k] == '\n')) {

						//printf("1---\n");

					// check if the acquired data is what we what
					if (readbuf[0] == 'G' && readbuf[1] == 'P' && readbuf[2] == 'R' && readbuf[3] == 'M' && readbuf[4] == 'C') {

						readbuf[k-1] = '\0';


						//
						// if we don't make a copy of the original gps_data,
						// then, the gps_data will be changed during the strtok function invocation.
						//
						strncpy(gps_data_cpy, readbuf, strlen(readbuf));

						p = strtok(gps_data_cpy, ",");

						while (p != NULL) {

							s++;
							p = strtok(NULL, ",");

						}

						if (s == 11) {

							printf("3---\n");

							if (calc_gps_cs(readbuf, k-1) == 0) {

							printf("4---\n");

								return 1;

							} else
								return 0;
						} else {
							return 0;
						}

					} else {

						return 0;

					}

					}

				}

				k++;

			}else {

				if (tmpdata[0] == '$') {

					pre1 = '$';

				}

			}
		//}
	}

	return 0;
}
