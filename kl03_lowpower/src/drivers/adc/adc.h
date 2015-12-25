/*
 * File:		adc.h
 * Purpose:     
 *
 * Notes:
 */

#ifndef __ADC_H__
#define __ADC_H__

/********************************************************************/


extern void InitADC( void );

extern unsigned char ADCAutoCalibration(void);

extern unsigned char TriggerADC(unsigned char Channel);

extern unsigned int ReadADCPoll(void);

extern unsigned char  ADCHardwareTriggerSelect(unsigned char HardwareTriggerSource);



/********************************************************************/

#endif /* __ADC_H__ */
