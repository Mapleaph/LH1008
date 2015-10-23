#ifndef __LIBIM_H__
#define __LIBIM_H__

void im_init();
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

long double ieee_to_decimal(char bin_str[]);
char* hex_to_bin(char* hex_bin);
int calc_im_cs(int buf[]);
void test_im();

#endif
