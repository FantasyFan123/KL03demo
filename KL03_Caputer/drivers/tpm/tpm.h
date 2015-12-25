/*
 * File:		tpm.h
 * Purpose:     Provide common tpm routines for PWM
 *
 * Notes:
 */

#ifndef __tpm_H__
#define __tpm_H__

/********************************************************************/
void TPM_PORT_Init(void);
void TPM0_Init(void);
void TPM1_Init(void);
void TPM0_IRQHandler(void);
void TPM1_IRQHandler(void);
/********************************************************************/

#endif /* __tpm_H__ */
