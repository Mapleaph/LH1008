#ifndef __LIBCOM_H__
#define __LIBCOM_H__

void sendtocom(unsigned long addr, unsigned char *buf, int size);
void sendtocom0(unsigned char* buf);
unsigned int recfromcom(unsigned long addr, unsigned char read_data[]);
void set_loop_mode();
void set_normal_mode();
void com_init();
void test_com();
void test_com_wf(unsigned long comAddr);

#endif
