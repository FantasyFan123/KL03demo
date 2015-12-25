/*
 * File:        tpm.c
 * Purpose:     Provide common tpm routines for PWM or capture
 *
 * Notes: jinhui      
 *              
 */
#include "common.h"
#include "tpm.h"

/***************************************************************************//*!
 * @brief   PORT module initialization.
 ******************************************************************************/
void TPM_PORT_Init(void)
{
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK| SIM_SCGC5_PORTB_MASK; //enable port clocks

  PORTB_PCR11 = PORT_PCR_ISF_MASK |PORT_PCR_MUX(0x2)|PORT_PCR_DSE_MASK;//select PA6 as pwm output
}

/***************************************************************************//*!
 * @brief   TPM0 module initialization.
 ******************************************************************************/
void TPM0_Init(void)
{
  SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1);	  //48MHz as tpm clock source
  SIM_SCGC6 |= SIM_SCGC6_TPM0_MASK| SIM_SCGC6_TPM1_MASK;   //enable tpm clock

  TPM0_CNT = 0;
  TPM0_MOD = 0x4B00;
  TPM0_SC = TPM_SC_TOIE_MASK|TPM_SC_CMOD(1);
  TPM0_C0SC = TPM_CnSC_MSB_MASK| TPM_CnSC_ELSB_MASK;
  TPM0_C0V = 0x2580;
}

