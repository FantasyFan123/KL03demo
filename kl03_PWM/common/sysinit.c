/*
 * File:        sysinit.c
 * Purpose:     Kinetis Configuration
 *              Initializes processor to a default state
 *
 * Notes:
 *
 */

#include "common.h"
#include "sysinit.h"
#include "uart.h"
#include "mcg_lite.h"
#include "freedom.h"
/********************************************************************/

/* Actual system clock frequency */
int mcg_clk_hz;
int mcg_clk_khz;
int core_clk_khz;
int periph_clk_khz;
int uart0_clk_khz;

/********************************************************************/
void sysinit (void)
{
    /* Enable all of the port clocks. These have to be enabled to configure
     * pin muxing options, so most code will need all of these on anyway.
     */
		SIM_COPC = 0x00;  //close watchdog

    SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK);
    
    // releases hold with ACKISO:  Only has an effect if recovering from VLLS1, VLLS2, or VLLS3
    // if ACKISO is set you must clear ackiso before calling pll_init 
    //    or pll init hangs waiting for OSC to initialize
    // if osc enabled in low power modes - enable it first before ack
    // if I/O needs to be maintained without glitches enable outputs and modules first before ack.
    if (PMC_REGSC &  PMC_REGSC_ACKISO_MASK)
		{
				PMC_REGSC |= PMC_REGSC_ACKISO_MASK;
		}
		
#ifdef MCG_HIRC_MODE_EN
    mcg_clk_hz = MCG_HIRC48();
    
    if(mcg_clk_hz==0)
    {
        mcg_clk_hz = MCG_IRC8M;
    }
  
  SIM_SOPT2 |= SIM_SOPT2_UART0SRC(1); // select the 48M clock  as UART0 clock source

		uart0_clk_khz = mcg_clk_hz;
#else
    /* default setting */
    mcg_clk_hz = MCG_IRC8M;
    SIM_SOPT2 |= SIM_SOPT2_UART0SRC(3); // select the MCGIRCLK as UART0 clock source
    uart0_clk_khz = MCG_IRC8M;
#endif
    
		SIM_CLKDIV1 = SIM_CLKDIV1_OUTDIV1(11);//outdiv1 divided by 12      jinhui 

	/*
         * Use the value obtained from the pll_init function to define variables
	 * for the core clock in kHz and also the peripheral clock. These
	 * variables can be used by other functions that need awareness of the
	 * system frequency.
	 */
    mcg_clk_khz = mcg_clk_hz / 1000;
    core_clk_khz = mcg_clk_khz / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV1_MASK) >> 28)+ 1);
    periph_clk_khz = core_clk_khz / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV4_MASK) >> 16)+ 1); 
      
    /* configure LPUART pin mux to PTB1,PTB2 */   // wenxue
  	PORTB_PCR1 = PORT_PCR_MUX(0x2); 
  	PORTB_PCR2 = PORT_PCR_MUX(0x2); 
      
    uart0_init (TERM_PORT,uart0_clk_khz,TERMINAL_BAUD);        
}
