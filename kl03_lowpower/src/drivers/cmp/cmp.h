/*
 * File:		cmp.h
 * Purpose:     Provide common ColdFire UART routines for polled serial IO
 *
 * Notes:
 */

#ifndef __CMP_H__
#define __CMP_H__

/********************************************************************/
// first instance not implemented yet.
//void cmp_init (CMP_MemMapPtr cmpch, int inp, int inn, int clk,int outen int dacval);
void cmp_init (CMP_MemMapPtr cmpch, unsigned char rising_falling )  ;
void cmp0_isr(void);

/********************************************************************/

#endif /* __CMP_H__ */
