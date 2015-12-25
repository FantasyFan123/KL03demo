/*
 * File:        tpm.c
 * Purpose:     Provide common tpm routines for PWM or capture
 *
 * Notes: jinhui      
 *              
 */
#include "common.h"
#include "tpm.h"

extern   volatile uint16 i16PulseWidth;
volatile int16 i16PulseWidth_0;
volatile int16 i16PulseWidth_1;
volatile int16 i16PulseWidth_2;

volatile int16 Flag_0;
volatile int16 Flag_1;

/***************************************************************************//*!
 * @brief   PORT module initialization.
 ******************************************************************************/
void TPM_PORT_Init(void)
{
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK| SIM_SCGC5_PORTB_MASK; //enable port clocks

	PORTB_PCR7 = PORT_PCR_ISF_MASK |PORT_PCR_MUX(0x2);//select PB7 as ptm1  ch0 input

}

/***************************************************************************//*!
 * @brief   TPM1 module initialization.
 ******************************************************************************/
void TPM1_Init(void)
{
	SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1);	  //48MHz as tpm clock source
	SIM_SCGC6 |= SIM_SCGC6_TPM1_MASK;   //enable tpm clock

  TPM1_CNT = 0;
  TPM1_MOD = 0xFFFF;
  TPM1_SC = TPM_SC_TOIE_SHIFT|TPM_SC_CMOD(1);
  TPM1_C0SC = TPM_CnSC_ELSA_MASK|TPM_CnSC_CHIE_MASK;
  TPM1_C0V = 0x00;
	enable_irq(18); 
	Flag_1 = 0;
	Flag_0 = 0;
}

/***************************************************************************//*!
 * @brief   TPM1 overflow interrupt service routine.
 ******************************************************************************/
void TPM1_IRQHandler(void)
{
  if((TPM1_SC & TPM_SC_TOF_MASK) == TPM_SC_TOF_MASK)
	{
		TPM1_SC	|= TPM_SC_TOF_MASK;
		Flag_0 = 1;
	}
	if((TPM1_C0SC & TPM_CnSC_CHF_MASK) == TPM_CnSC_CHF_MASK)
	{
		TPM1_C0SC |= TPM_CnSC_CHF_MASK;
	}
	if(Flag_1 == 0)
	{
		if(Flag_0 == 1)
			{
				i16PulseWidth_0 =  (int16)TPM1_C0V + 0xFFFF;
				Flag_0 = 0;
			}else{
				i16PulseWidth_0 =  (int16)TPM1_C0V;
			}
		Flag_1 = 1;
	} else {
		if(Flag_0 == 1)
			{
				i16PulseWidth_1 =  (int16)TPM1_C0V + 0xFFFF;
				Flag_0 = 0;
			}else{
				i16PulseWidth_1 =  (int16)TPM1_C0V;
			}
		Flag_1 = 0;
	}
	if(i16PulseWidth_1 > i16PulseWidth_0)
		{
			i16PulseWidth = i16PulseWidth_1 -i16PulseWidth_0;
		}else{
			i16PulseWidth = i16PulseWidth_0 -i16PulseWidth_1;
		}
}
/*****************************************************************************/

