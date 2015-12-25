/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2011-2012 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file pit.h
*
* @author B37531
*
* @version 0.0.1
*
* @date Aug 23, 2011
*
* @brief application entry point which performs application specific tasks. 
*
*******************************************************************************
*
*
******************************************************************************/

/******************************************************************************
* Constants and macros
******************************************************************************/

/******************************************************************************
* Global variables
******************************************************************************/

/*****************************************************************************//*!
+FUNCTION----------------------------------------------------------------
* @function name: PIT_Init
*
* @brief 
*		 
* @param  none 
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/


#ifndef PIT_H_
#define PIT_H_

#define PIT_TIMERS			200 //mS
#define PIT_MODULO			(mcg_clk_hz/1000)*PIT_TIMERS

void PIT_Isr( void );

void PIT_Init( unsigned char cChannel );


extern uint32_t g_ulCurrentTimeCount;

#endif /* PIT_H_ */
