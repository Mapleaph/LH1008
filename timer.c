#define timer_c

#include <stdlib.h>
#include <stdio.h>
#include <c6x.h>

/*F***************************************************************************
* NAME:   TIMER_Init()
*
* DESCRIPTION: Setup our timer that is used for all TEB examples.       
*       
* NOTES:
*   
*F***************************************************************************/

void TIMER_Init(void)
{
	*((unsigned int *)(0x01940000)) = 0x00008200;//ctl
	*((unsigned int *)(0x01940004)) = 0xffffffff;//prd
	*((unsigned int *)(0x01940008)) = 0x00000000;//cnt
}
/*L***************************************************************************
* NAME:   TIMER_DelayMsec()
*
* DESCRIPTION: Simple delay based on 200 MHz DSP clock.       
*       
*   
*L***************************************************************************/
/* Assume 200 MHz CPU, timer period is CPUCLK/4 for C67x */
void TIMER_DelayMsec(short msec)
{
	unsigned int timer_limit = (msec*50000);
//	unsigned int timer_start;
	unsigned int timer_current;
	*((unsigned int *)(0x01940000)) = 0x00008280;
	*((unsigned int *)(0x01940000)) = 0x000082c0;
	do
  	{
  		timer_current = (*((unsigned int *)(0x01940008)));
  	}while( (timer_current) < timer_limit);
	*((unsigned int *)(0x01940000)) = 0x00008200;
}
