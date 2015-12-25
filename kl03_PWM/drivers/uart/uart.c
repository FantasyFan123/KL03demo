/*
 * File:        uart.c
 * Purpose:     Provide common uart routines for serial IO
 *
 * Notes:       
 *              
 */

#include "common.h"
#include "uart.h"

/***************************************************************************
 * Begin UART0 functions
 **************************************************************************/
/********************************************************************/
/*
 * Initialize the uart for 8N1 operation, interrupts disabled, and
 * no hardware flow-control
 *
 * NOTE: Since the uarts are pinned out in multiple locations on most
 *       Kinetis devices, this driver does not enable uart pin functions.
 *       The desired pins should be enabled before calling this init function.
 *
 * Parameters:
 *  uartch      uart channel to initialize
 *  sysclk      uart module Clock in kHz(used to calculate baud)
 *  baud        uart baud rate
 */
void uart0_init (LPUART_MemMapPtr uartch, int uart0clk, int baud)
{
    register uint16 sbr;
    uint8 temp;
    
    SIM_SCGC5 |= SIM_SCGC5_LPUART0_MASK;
  

      /* Configure the uart for 8-bit mode, no parity */
    LPUART_CTRL_REG(uartch) = 0;	/* We need all default settings, so entire register is cleared */
    
      /* Calculate baud settings */
    temp = ((LPUART_BAUD_REG(uartch) & LPUART_BAUD_OSR_MASK)>>24) + 1;
    sbr = (uint16)((uart0clk)/(baud * (temp)));
    
    LPUART_BAUD_REG(uartch) &= ~LPUART_BAUD_SBR_MASK;
    LPUART_BAUD_REG(uartch) |= LPUART_BAUD_SBR(sbr);    

    /* Enable receiver and transmitter */
    LPUART_CTRL_REG(uartch) |= (LPUART_CTRL_TE_MASK
              		  | LPUART_CTRL_RE_MASK );
    
}
/********************************************************************/
/*
 * Wait for a character to be received on the specified uart
 *
 * Parameters:
 *  channel      uart channel to read from
 *
 * Return Values:
 *  the received character
 */
char uart0_getchar (LPUART_MemMapPtr channel)
{
      /* Wait until character has been received */
      while (!(LPUART_STAT_REG(channel) & LPUART_STAT_RDRF_MASK));
    
      /* Return the 8-bit data from the receiver */
      return LPUART_DATA_REG(channel);
}
/********************************************************************/
/*
 * Wait for space in the uart Tx FIFO and then send a character
 *
 * Parameters:
 *  channel      uart channel to send to
 *  ch			 character to send
 */ 
void uart0_putchar (LPUART_MemMapPtr channel, char ch)
{
      /* Wait until space is available in the FIFO */
      while(!(LPUART_STAT_REG(channel) & LPUART_STAT_TDRE_MASK));
    
      /* Send the character */
      LPUART_DATA_REG(channel) = (uint8)ch;
    
 }
/********************************************************************/
/*
 * Check to see if a character has been received
 *
 * Parameters:
 *  channel      uart channel to check for a character
 *
 * Return values:
 *  0       No character received
 *  1       Character has been received
 */
int uart0_getchar_present (LPUART_MemMapPtr channel)
{
    return (LPUART_STAT_REG(channel) & LPUART_STAT_RDRF_MASK);
}
/********************************************************************/
