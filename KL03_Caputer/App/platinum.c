/*
 * File:		platinum.c
 * Purpose:	Main process   
 * Author:	jinhui wu  (tel:18811006627)
 */

#include "common.h"
#include "tpm.h"

/***************************************************************************//*!
 * @brief   macro define
 ******************************************************************************/


/***************************************************************************//*!
 * @brief   globel temp.
 ******************************************************************************/
volatile uint16 i16PulseWidth; 

/******************************************************************************/
int main (void)
{
	char temp_tpm;
	
  TPM_PORT_Init();
  TPM1_Init();

	while(1)
	{
    printf("\nPress any key to detect pulse width");
    temp_tpm = in_char();
    printf("\nMeasured Pulse width: %d TPM1 cycles\n\n", i16PulseWidth);
	} 
}

