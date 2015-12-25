/*
 * File:		uart.h
 * Purpose:     Provide common ColdFire uart routines for polled serial IO
 *
 * Notes:
 */

#ifndef __uart_H__
#define __uart_H__

/********************************************************************/



void uart0_init (LPUART_MemMapPtr uartch, int sysclk, int baud);
char uart0_getchar (LPUART_MemMapPtr channel);
void uart0_putchar (LPUART_MemMapPtr channel, char ch);
int uart0_getchar_present (LPUART_MemMapPtr channel);

/********************************************************************/

#endif /* __uart_H__ */
