/*
 * File:		SCI.h
 * Purpose:     Provide common ColdFire SCI routines for polled serial IO
 *
 * Notes:
 */

#ifndef __SCI_H__
#define __SCI_H__

/********************************************************************/

/** SCI MemMapPtr Declaration **/
//#define UART1_BASE_PTR                           ((SCI_MemMapPtr)0x4006B000u)
//#define UART2_BASE_PTR                           ((SCI_MemMapPtr)0x4006C000u)


void SCI_init (SCI_MemMapPtr SCIch, int sysclk, int baud);
char SCI_getchar (SCI_MemMapPtr channel);
void SCI_putchar (SCI_MemMapPtr channel, char ch);
int SCI_getchar_present (SCI_MemMapPtr channel);

/********************************************************************/

#endif /* __SCI_H__ */
