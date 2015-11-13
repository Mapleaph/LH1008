#ifndef __LIBGPS_H__
#define __LIBGPS_H__

int get_gps_data(unsigned char readbuf[]);
char* get_gps_date(char* gps_data);
char* get_gps_time(char* gps_data);
void get_gps_latitude(char* gps_data, char* latitude[]);
void get_gps_longitude(char* gps_data, char* longitude[]);
double get_gps_spd(char* gps_data);
double get_gps_cog(char* gps_data);

int get_dec_part(double num);
double get_frac_part(double num);

int calc_gps_cs(char readbuf[], unsigned int length);

void test_gps();

#endif
