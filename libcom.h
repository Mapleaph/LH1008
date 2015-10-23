#ifndef __LIBCOM_H__
#define __LIBCOM_H__

void sendtocom(unsigned long addr, unsigned char *buf, int size);
void sendtocom0(unsigned char* buf);
unsigned int recfromcom(unsigned long addr, unsigned int read_data[]);
void com_init();
void test_com();

#endif
