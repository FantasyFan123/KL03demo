/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2011-2012 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file tsi.h
*
* @author B37531
*
* @version 0.0.1
*
* @date Aug 23, 2011
*
* @brief application entry point which performs application specific tasks. 
*
******************************************************************************/
#ifndef TSI_H_
#define TSI_H_


/******************************************************************************
* Includes
******************************************************************************/

/******************************************************************************
* Constants
******************************************************************************/

/******************************************************************************
* Macros
******************************************************************************/
//TSI idle exceed this time,will enter into stop mode
#define TSI_IDLE_TIME		5 //unit is s
                                      
/* define prescaler of the output electrode oscillator */
#define TSI_CS1_PS_1		(0)
#define TSI_CS1_PS_2		(1)
#define TSI_CS1_PS_4        (2)
#define TSI_CS1_PS_8       	(3)
#define TSI_CS1_PS_16       (4)
#define TSI_CS1_PS_32       (5)
#define TSI_CS1_PS_64      	(6)
#define TSI_CS1_PS_128     	(7)

/* define the scan number for each electrode */
#define TSI_CS1_NSCN_1		0
#define TSI_CS1_NSCN_2		1
#define TSI_CS1_NSCN_3		2
#define TSI_CS1_NSCN_4		3
#define TSI_CS1_NSCN_5		4
#define TSI_CS1_NSCN_6		5
#define TSI_CS1_NSCN_7		6
#define TSI_CS1_NSCN_8		7
#define TSI_CS1_NSCN_9		8
#define TSI_CS1_NSCN_10		9
#define TSI_CS1_NSCN_11		10
#define TSI_CS1_NSCN_12		11
#define TSI_CS1_NSCN_13		12
#define TSI_CS1_NSCN_14		13
#define TSI_CS1_NSCN_15		14
#define TSI_CS1_NSCN_16		15
#define TSI_CS1_NSCN_17		16
#define TSI_CS1_NSCN_18		17
#define TSI_CS1_NSCN_19		18
#define TSI_CS1_NSCN_20		19
#define TSI_CS1_NSCN_21		20
#define TSI_CS1_NSCN_22		21
#define TSI_CS1_NSCN_23		22
#define TSI_CS1_NSCN_24		23
#define TSI_CS1_NSCN_25		24
#define TSI_CS1_NSCN_26		25
#define TSI_CS1_NSCN_27		26
#define TSI_CS1_NSCN_28		27
#define TSI_CS1_NSCN_29		28
#define TSI_CS1_NSCN_30		29
#define TSI_CS1_NSCN_31		30
#define TSI_CS1_NSCN_32		31



/* define the oscillator's voltage rails */
#define TSI_CS2_DVOLT_00			(0)
#define TSI_CS2_DVOLT_01			(1)
#define TSI_CS2_DVOLT_10			(2)
#define TSI_CS2_DVOLT_11			(3)

/* define the oscillator charge current */
#define TSI_CONSTANT_CURRENT_500nA			(0)
#define TSI_CONSTANT_CURRENT_1uA			(1)
#define TSI_CONSTANT_CURRENT_2uA			(2)
#define TSI_CONSTANT_CURRENT_4uA			(3) 
#define TSI_CONSTANT_CURRENT_8uA			(4) 
#define TSI_CONSTANT_CURRENT_16uA			(5)
#define TSI_CONSTANT_CURRENT_32uA		   	(6)
#define TSI_CONSTANT_CURRENT_64uA		   	(7)

#define TSI_MODE_CAP_SENSE								0x00
#define TSI_MODE_SINGLE_THRESHOLD						0x04
#define TSI_MODE_SINGLE_THRESHOLD_FREQUENCY_LIMITED		0x08
#define TSI_MODE_AUTO_NOISE_DETECTION					0x0c


#define TSI_AMCLKS_LPOCLK					0
#define TSI_AMCLKS_IRCLK					1
#define TSI_AMCLKS_ERCLK					2
#define TSI_AMCLKS_INVALID					3


#define TSI_AMPSC_DIVIDER_1			0
#define TSI_AMPSC_DIVIDER_2			1
#define TSI_AMPSC_DIVIDER_4			2
#define TSI_AMPSC_DIVIDER_8			3
#define TSI_AMPSC_DIVIDER_16		4
#define TSI_AMPSC_DIVIDER_32		5
#define TSI_AMPSC_DIVIDER_64		6
#define TSI_AMPSC_DIVIDER_128		7

#define PERIOD_SCAN_TIME			20 // mS
#define PERIOD_NORMAL_SCAN_MODULO	PERIOD_SCAN_TIME
#define PERIOD_LP_SCAN_MODULO		PERIOD_SCAN_TIME*50


#define TSI_LOW_THRESHOLD			4000
#define TSI_HIGH_THRESHOLD			6000


#define true					1
#define false					0

//define key parameter
#define ELECTRODE_NUM				2
#define KEY_OVERTIME				2
#define KEY_ON_SENSITIVITY			1000u
#define KEY_OFF_SENSITIVITY			200u

// deine Key state
#define KEY_RELEASED				0x00
#define KEY_TOUCHED					0x01

//define TSI work state,
#define TSI_WORK_STATE_NULL					0x00
#define TSI_WORK_STATE_NORMAL		        0x10

//define key check mode
#define KEY_CHECK_MODE_NORMAL		        0x00
#define KEY_CHECK_MODE_STANDBY		        0x01



/******************************************************************************
* Types
******************************************************************************/
typedef struct
{
	unsigned short MinValue;
	unsigned short MaxValue;
	unsigned short Average;
	unsigned short Delta;
}ELECTRODE_CHECK_INFO;

typedef struct
{
	unsigned short BaseLine;
	short Delta;
	short Samples;
	unsigned char  Channel;
	unsigned char  Count;
	unsigned char  State;
}ELECTRODE_WORK_INFO;

/******************************************************************************
* Global variables
******************************************************************************/
extern unsigned char g_KeyCheckMode;
/******************************************************************************
* Global functions
******************************************************************************/
extern void TSI_Init( void );
extern void TSI_Processing( void );
extern void KEY1_Processing( void );
extern void KEY2_Processing( void );
extern void TSI_Check( void );
extern void TSI_EnterIntoStopMode( void );
extern void MCU_EnterStopMode( void );
extern void TSI_SynTime( void );
void EnableOthersInterrupt( void );
void DisableOthersInterrupt( void );


#endif /* TSI_H_ */
