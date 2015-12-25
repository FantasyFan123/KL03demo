/*
 * File:        adc.c
 * Purpose:     Provide common adc routines
 *
 * Notes:       
 *              
 */

#include "common.h"
#include "adc.h"

/****************************************************************
                  initial the ADC module
*****************************************************************/
void InitADC( void )
{
	SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;  // enable ADC0 clock

  	ADC0_CFG1 = ADC_CFG1_ADIV(1)|
  				ADC_CFG1_MODE(2)|
  				ADC_CFG1_ADICLK(3);
  	
}
        
          
/****************************************************************
                      software trigger channel x
*****************************************************************/
unsigned char TriggerADC(unsigned char Channel)
{
  
  if(Channel == 0x1B)                                          // Bandgap is selected as input
      PMC_REGSC |= PMC_REGSC_BGBE_MASK;                        // Bandgap buffer enabled
   ADC0_SC1A = Channel;                   // update the channel value
   return 1;
}

/****************************************************************
                      ADC Read
*****************************************************************/
unsigned int ReadADCPoll(void)
{
    while((ADC0_SC1A & ADC_SC1_COCO_MASK) == 0);
    //ADC0_SC1A &= ~ ADC0_SC1A_COCO_MASK;
    return ADC0_RA;
}

void ADC_ISR(void)
{
  printf("\n ADC sample data: %x.\n", ADC0_RA);
}
