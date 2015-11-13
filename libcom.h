#ifndef __LIBCOM_H__
#define __LIBCOM_H__

void sendtocom(unsigned long addr, unsigned char* buf, int size);
void sendtocom0(unsigned char* buf);
unsigned int recfromcom(unsigned long addr, unsigned char read_data[]);
void set_loop_mode();
void set_normal_mode();
void com_init();
void test_com();
void test_com_wf(unsigned long comAddr);

#define COM7ADDR 0xa0051000
#define COM1ADDR 0xa0050000
#define COM2ADDR 0xa0050020
#define COM3ADDR 0xa0050040
#define COM4ADDR 0xa0050060
#define COM5ADDR 0xa0050080
#define COM6ADDR 0xa00500a0

#define DBGADDR COM7ADDR
#define IMADDR COM1ADDR
#define GPSADDR COM1ADDR

#endif
