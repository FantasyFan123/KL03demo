/*
 * File:		platinum.c
 * Purpose:	Main process   
 * Author:	jinhui wu  (tel:18811006627)
 */

#include "common.h"
#include "tpm.h"
#include "lptmr.h"
#include "smc.h"
/******************************************************************************/
int main (void)
{
	int i;
  //TPM_PORT_Init();
  //TPM0_Init();
	
	lptmr_init(10000,1);
	//printf("\nThe PWM Pluse 25%% 10kHz");
	enter_vlps();
	while(1)
	{
    i++;
		
	} 
}

