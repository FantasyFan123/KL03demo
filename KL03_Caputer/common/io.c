/*
 * File:		io.c
 * Purpose:		Serial Input/Output routines
 *
 * Notes:     TERMINAL_PORT defined in <board>.h
 */

#include "common.h"
#include "uart.h"

/********************************************************************/
char in_char (void)
{
    return uart0_getchar(LPUART0_BASE_PTR);
 
}
/********************************************************************/
void out_char (char ch)
{
    uart0_putchar(LPUART0_BASE_PTR, ch);
}

/********************************************************************/
int char_present (void)
{
    return uart0_getchar_present(LPUART0_BASE_PTR);
   
}
/********************************************************************/
