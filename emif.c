/*****************************************************************************
* NOTES: Assumptions:
*             TMS320C6713 TEB 
*             AM29LV040B flash connected to EMIF, CE1
*             SDRAM connected to EMIF, CE0
*
****************************************************************************/
#define emif_c

#include <stdlib.h>
#include <stdio.h>
#include <c6x.h>
#include "emif.h"
#include "csl_emif.h"

/*F***************************************************************************
* NAME:   EMIF_Init()
*
* DESCRIPTION: Setup the DSK EMIF      
*       
* NOTES:
*   
*F***************************************************************************/

void EMIF_Init( void )
{
	#define EMIF_GCTL       0x01800000
	#define EMIF_CE1        0x01800004
	#define EMIF_CE0        0x01800008
	#define EMIF_CE2        0x01800010
	#define EMIF_CE3        0x01800014
	#define EMIF_SDRAMCTL   0x01800018
	#define EMIF_SDRAMTIMING  0x0180001C
  	#define EMIF_SDRAMEXT     0x01800020

  	*(int *)EMIF_GCTL = 0x00000078;/* EMIF global control register     enable clk1 clk2    */
  	*(int *)EMIF_CE1 = 0x02208822; /* 0x02208802 0xc276ff06 , CE1 - 8-bit asynch access after boot*/
  	*(int *)EMIF_CE0 = 0xffffff9f; /* CE0 - SDRAM                          */
   	*(int *)EMIF_CE2 = 0x2fe2bf22; // 2fe2bf22 22a28a22
   	*(int *)EMIF_CE3 = 0x2fe2bf22; 
  	*(int *)EMIF_SDRAMCTL = 0x6344a000; /* SDRAM control register (100 MHz)*/
  	*(int *)EMIF_SDRAMTIMING = 0x00000578; /* SDRAM Timing register        */
	return;
}
