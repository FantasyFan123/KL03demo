/*
 * File:        lptmr.c
 * Purpose:     Provide common low power timer functions
 *
 * Notes:       Right now only function provided is used
 *              to generate a delay in ms. This driver
 *              could be expanded to include more functions
 *              in the future.
 *
 */


#include "common.h"
#include "lptmr.h"


extern int re_init_clk;
extern int clock_freq_hz;

/********************************************************************/
/*
 * Initialize the low power time to provide a delay measured in ms.
 *
 *
 * Parameters:
 *  count_val   number of ms to delay
 *
 * Returns:
 * None
 */
void time_delay_ms(uint32 count_val)
{
  /* Make sure the clock to the LPTMR is enabled */
  SIM_SCGC5|=SIM_SCGC5_LPTMR_MASK;

  /* Reset LPTMR settings */
  LPTMR0_CSR=0;

  /* Set the compare value to the number of ms to delay */
  LPTMR0_CMR = count_val;

  /* Set up LPTMR to use 1kHz LPO with no prescaler as its clock source */
  LPTMR0_PSR = LPTMR_PSR_PCS(1)|LPTMR_PSR_PBYP_MASK;

  /* Start the timer */
  LPTMR0_CSR |= LPTMR_CSR_TEN_MASK;

  /* Wait for counter to reach compare value */
  while (!(LPTMR0_CSR & LPTMR_CSR_TCF_MASK));

  /* Disable counter and Clear Timer Compare Flag */
  LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK;

  return;
}

void lptmr_isr(void)
{

  // printf("\n****LPT ISR entered*****\r\n");
  // write 1 to TCF to clear the LPT timer compare flag
  LPTMR0_CSR |=  LPTMR_CSR_TCF_MASK;   
}
/*******************************************************************************
*
*   PROCEDURE NAME:
*       lptmr_init -
*
*******************************************************************************/
void lptmr_init(int count, int clock_source)
{
    SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;
    // disable LPTMR
    LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK; 
    // select LPO for RTC and LPTMR
    LPTMR0_PSR = ( LPTMR_PSR_PRESCALE(0) // 0000 is div 2
                 | LPTMR_PSR_PBYP_MASK  // clock feeds directly to LPT
                 | LPTMR_PSR_PCS(clock_source)) ; // use the choice of clock
              
    LPTMR0_CMR = LPTMR_CMR_COMPARE(count);  //Set compare value
	// clear flag
	LPTMR0_CSR |=  LPTMR_CSR_TCF_MASK; 
	
    LPTMR0_CSR =(  LPTMR_CSR_TCF_MASK   // Clear any pending interrupt
                 | LPTMR_CSR_TIE_MASK   // LPT interrupt enabled
                 );
	
    LPTMR0_CSR |= LPTMR_CSR_TEN_MASK;   //Turn on LPT and start counting
    
    enable_irq(INT_LPTimer - 16);
}
