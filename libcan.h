/*SJA1000 registers*/
#define REG_MOD		0x00	/*mode register*/
#define REG_CMR		0x01	/*command register*/
#define	REG_SR		0x02	/*status register*/ 
#define REG_IR		0x03	/*interrupt register*/
#define REG_IER		0x04	/*interrupt enable register*/

#define REG_BTR0	0x06	/*bus timing 0 register*/
#define	REG_BTR1	0x07	/*bus timing 1 register*/
#define REG_OCR		0x08	/*output control register*/

#define REG_ALC		0x0B	/*arbitration lost capture register*/
#define REG_ECC		0x0C	/*error code capture register*/
#define REG_EWL		0x0D	/*eror warning limit register*/

#define REG_RXERR	0x0E	/*RX error counter register*/
#define REG_TXERR	0x0F	/*TX error counter register*/
#define REG_ACCC0	0x10	/*acceptance code 0*/
#define REG_ACCC1	0x11	/*acceptance code 1*/
#define REG_ACCC2	0x12	/*acceptance code 2*/
#define REG_ACCC3	0x13	/*acceptance code 3*/
#define REG_ACCM0	0x14	/*acceptance mask 0*/
#define REG_ACCM1	0x15	/*acceptance mask 1*/
#define REG_ACCM2	0x16	/*acceptance mask 2*/
#define REG_ACCM3	0x17	/*acceptance mask 3*/
#define REG_RMC		0x1D	/*RX message counter*/
#define REG_RBSA	0x1E	/*RX buffer start address*/

#define REG_CDR		0x1F	/*clock divider*/	

#define REG_FI		0x10	/*frame information register*/
#define SFF_BUF		0x13	/*standard frame format data start address*/
#define EFF_BUF		0x15	/*extended frame format data start address*/

#define CAN_RAM		0x20	/*fifo internal RAM address 0*/

/*mode register*/
#define MOD_RM		(1<<0)	/*reset mode*/
#define MOD_LOM		(1<<1)	/*listen only mode*/
#define MOD_STM		(1<<2)	/*self test mode*/
#define MOD_AFM		(1<<3)	/*acceptance filter mode*/
#define MOD_SM		(1<<4)	/*sleep mode*/

/*commands*/
#define CMD_TR		(1<<0)	/*transmission request*/
#define CMD_AT		(1<<1)	/*abort transmission*/
#define CMD_RRB		(1<<2)	/*release receive buffer*/
#define CMD_CDO		(1<<3)	/*clear data overrun*/
#define CMD_SRR		(1<<4)	/*self reception request*/

/*status*/
#define SR_RBS		(1<<0)	/*receive buffer status*/
#define SR_DOS		(1<<1)	/*data overrun status*/
#define SR_TBS		(1<<2)	/*transmit buffer status*/
#define SR_TCS		(1<<3)	/*transmission complete status*/
#define SR_RS		(1<<4)	/*receive status*/
#define SR_TS		(1<<5)	/*transmit status*/
#define SR_ES		(1<<6)	/*error status*/
#define SR_BS		(1<<7)	/*bus status*/

/*interrupt source*/
#define	IRQ_RI		(1<<0)	/*receive interrupt*/
#define IRQ_TI		(1<<1)	/*transmit interrupt*/
#define IRQ_EI		(1<<2)	/*error warning interrupt*/
#define IRQ_DOI		(1<<3)	/*data overrun interrupt*/
#define IRQ_WUI		(1<<4)	/*wake-up interrupt*/
#define IRQ_EPI		(1<<5)	/*error passive interrupt*/
#define IRQ_ALI		(1<<6)	/*arbitration lost interrupt*/
#define IRQ_BEI		(1<<7)	/*bus error interrupt*/

#define IRQ_ALL		0xFF	/*enable all interrupts*/
#define IRQ_OFF		0x00	/*disable all interrupts*/