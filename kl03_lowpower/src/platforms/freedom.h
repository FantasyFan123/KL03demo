/*
 * File:        freedom.h
 * Purpose:     Kinetis freedom CPU card definitions
 *
 * Notes:
 */

#ifndef __FREEDOM_H__
#define __FREEDOM_H__

#include "mcg_lite.h"

/********************************************************************/

/* Global defines to use for all Tower cards */
#define DEBUG_PRINT
#define CPU_MKL03Z32LF4


/*
* Input Clock Info
*/
#define CLK0_FREQ_HZ        32768
#define CLK0_TYPE           CRYSTAL

#define MCG_IRC8M           8000000
#define MCG_IRC2M           2000000
#define MCG_IRC48M          48000000
  /* Serial Port Info */
#define MCG_HIRC_MODE_EN		


/* Serial Port Info */
/* Uses UART0 for both OSJTAG and TWR-SER Tower card */
#define TERM_PORT           LPUART0_BASE_PTR

/** USE_UART0 must be defined if you want to use UART0 **/
#define USE_UART0
#define UART_PIN_PTB_1_2_
#ifdef BOOTLOADER
  #define TERMINAL_BAUD       19200
#else
  #define TERMINAL_BAUD       19200
#endif
  #undef  HW_FLOW_CONTROL


#define true                            1
#define false                           0

#endif /* __FREEDOM_H__ */
