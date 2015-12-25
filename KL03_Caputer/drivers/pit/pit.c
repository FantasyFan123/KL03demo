/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2011-2012 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file pit.c
*
* @author B37531
*
* @version 0.0.1
*
* @date Mar 5, 2012
*
* @brief application entry point which performs application specific tasks. 
*
*******************************************************************************
*
* provide TSI driver,complete electrode scan and processing,support stop mode
******************************************************************************/
#include "common.h"
#include "pit.h"
#include "sysinit.h"

/******************************************************************************
* Constants and macros
******************************************************************************/


/******************************************************************************
* Global variables
******************************************************************************/
static uint8_t m_ucTimerCount = 0;
/******************************************************************************
* Global variables
******************************************************************************/
uint32_t g_ulCurrentTimeCount;
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
void PIT_Init( unsigned char cChannel )
{
	SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;
	enable_irq(22);
	PIT_MCR = 0x00;
	PIT_LDVAL_REG(PIT_BASE_PTR,cChannel) = PIT_MODULO;
	PIT_TCTRL_REG(PIT_BASE_PTR,cChannel) = 
						PIT_TCTRL_TEN_MASK|  // enable PIT
						PIT_TCTRL_TIE_MASK;  // enable interrupt
}


/*****************************************************************************//*!
+FUNCTION----------------------------------------------------------------
* @function name: PIT_Isr
*
* @brief 
*		 
* @param  none 
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/

void PIT_Isr( void )
{
	if( PIT_TFLG0 )
	{
		PIT_TFLG0 = 0x01;
	
		if( m_ucTimerCount < 5 )
		{
			m_ucTimerCount ++;
		}
		else
		{
			m_ucTimerCount = 0;
			g_ulCurrentTimeCount ++;
		}
	}
}





