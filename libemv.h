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

void open_EMV(int n); // open the n-th EMV (1~12)
void close_EMV(int n); // close the n-th EMV (1~12)

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

int get_EMV();
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

void set_EMV(int val);

void open_EMV_All();
void close_EMV_All();
void init_EMV(); // initialization for all valves

void test_EMV();
