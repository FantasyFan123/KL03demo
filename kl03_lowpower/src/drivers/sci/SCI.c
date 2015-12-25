/*
 * File:        SCI.c
 * Purpose:     Provide common SCI routines for serial IO
 *
 * Notes:       
 *              
 */

#include "common.h"
#include "SCI.h"

/********************************************************************/
/*
 * Initialize the SCI for 8N1 operation, interrupts disabled, and
 * no hardware flow-control
 *
 * NOTE: Since the SCIs are pinned out in multiple locations on most
 *       Kinetis devices, this driver does not enable SCI pin functions.
 *       The desired pins should be enabled before calling this init function.
 *
 * Parameters:
 *  SCIch      SCI channel to initialize
 *  sysclk      SCI module Clock in kHz(used to calculate baud)
 *  baud        SCI baud rate
 */
void SCI_init (SCI_MemMapPtr SCIch, int sysclk, int baud)
{
    register uint16 sbr;
    uint8 temp;
    
	/* Enable the clock to the selected SCI */    
    if (SCIch == SCI1_BASE_PTR)
	SIM_SCGC4 |= SIM_SCGC4_UART1_MASK;
    else
    	SIM_SCGC4 |= SIM_SCGC4_UART2_MASK;
                                
    /* Make sure that the transmitter and receiver are disabled while we 
     * change settings.
     */
    SCI_C2_REG(SCIch) &= ~(SCI_C2_TE_MASK
				| SCI_C2_RE_MASK );

    /* Configure the SCI for 8-bit mode, no parity */
    SCI_C1_REG(SCIch) = 0;	/* We need all default settings, so entire register is cleared */
    
    /* Calculate baud settings */
    sbr = (uint16)((sysclk*1000)/(baud * 16));
        
    /* Save off the current value of the SCIx_BDH except for the SBR field */
    temp = SCI_BDH_REG(SCIch) & ~(SCI_BDH_SBR(0x1F));
    
    SCI_BDH_REG(SCIch) = temp |  SCI_BDH_SBR(((sbr & 0x1F00) >> 8));
    SCI_BDL_REG(SCIch) = (uint8)(sbr & SCI_BDL_SBR_MASK);
  
    /* Enable receiver and transmitter */
	SCI_C2_REG(SCIch) |= (SCI_C2_TE_MASK
				| SCI_C2_RE_MASK );
}
/********************************************************************/
/*
 * Wait for a character to be received on the specified SCI
 *
 * Parameters:
 *  channel      SCI channel to read from
 *
 * Return Values:
 *  the received character
 */
char SCI_getchar (SCI_MemMapPtr channel)
{
    /* Wait until character has been received */
    while (!(SCI_S1_REG(channel) & SCI_S1_RDRF_MASK));
    
    /* Return the 8-bit data from the receiver */
    return SCI_D_REG(channel);
}
/********************************************************************/
/*
 * Wait for space in the SCI Tx FIFO and then send a character
 *
 * Parameters:
 *  channel      SCI channel to send to
 *  ch			 character to send
 */ 
void SCI_putchar (SCI_MemMapPtr channel, char ch)
{
    /* Wait until space is available in the FIFO */
    while(!(SCI_S1_REG(channel) & SCI_S1_TDRE_MASK));
    
    /* Send the character */
    SCI_D_REG(channel) = (uint8)ch;
 }
/********************************************************************/
/*
 * Check to see if a character has been received
 *
 * Parameters:
 *  channel      SCI channel to check for a character
 *
 * Return values:
 *  0       No character received
 *  1       Character has been received
 */
int SCI_getchar_present (SCI_MemMapPtr channel)
{
    return (SCI_S1_REG(channel) & SCI_S1_RDRF_MASK);
}
/********************************************************************/
    
