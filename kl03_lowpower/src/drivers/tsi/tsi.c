/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2011-2012 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file tsi.c
*
* @author B37531
*
* @version 0.0.1
*
* @date Mar 7, 2012
*
* @brief application entry point which performs application specific tasks. 
*
*******************************************************************************
*
* provide TSI driver,complete electrode scan and processing,support stop mode
******************************************************************************/
#include "common.h"
#include "tsi.h"
#include "rtc.h"
#include "tower.h"
#include "lptmr.h"
#include "pit.h"
/******************************************************************************
* Constants and macros
******************************************************************************/

/******************************************************************************
* Global variables
******************************************************************************/
uint8_t g_KeyCheckMode;
/******************************************************************************
* Local variables
******************************************************************************/
void (* const KeyHandle[ELECTRODE_NUM])(void) =
{
	KEY1_Processing,	/* call back functions defined externally */
	KEY2_Processing,
};
/*****************************************************************************
* Local types
******************************************************************************/

/******************************************************************************
* Local variables
******************************************************************************/
static uint32_t m_ulIdleTimeCount;
static ELECTRODE_WORK_INFO	m_ElecWorkInfo[ELECTRODE_NUM];
static uint8_t m_ucTSI_WorkState;
static uint8_t m_ucSignalChangeFlag;
static uint8_t m_bScanCompleteFlag;
static uint8_t m_ucScanningIndex;
static uint8_t m_bOutOfRangeFlag;
/******************************************************************************
* Local functions
*****************************************************************************/
void TSI_VaribleInit( void );
void TSI_Init( void );

/******************************************************************************
* Global functions
******************************************************************************/
void TSI_Init( void );
void TSI_Processing( void );
void KEY1_Processing( void );
void KEY2_Processing( void );
void TSI_Check( void );
void TSI_ISR ( void );
void TSI_CheckState( void );
/******************************************************************************
* Local function prototypes
******************************************************************************/



void TSI_VaribleInit( void )
{
	unsigned char i;
	m_ucTSI_WorkState = TSI_WORK_STATE_NULL;
	g_KeyCheckMode = KEY_CHECK_MODE_NORMAL;
	for(i=0;i<ELECTRODE_NUM;i++)
	{
		m_ElecWorkInfo[i].State = KEY_RELEASED;
		m_ElecWorkInfo[i].BaseLine = 0;
	}

	// initial TSI channel
	m_ElecWorkInfo[0].Channel = 0;
	m_ElecWorkInfo[1].Channel = 1;

	// scanning index clear 0
	m_ucScanningIndex = 0;
	m_bScanCompleteFlag = false;
}
/*****************************************************************************//*!
   +FUNCTION----------------------------------------------------------------
   * @function name: TSI_Init
   *
   * @brief TSI module initialization
   *        
   * @param  none 
   *
   * @return none
   *
   * @ Pass/ Fail criteria: none
   *****************************************************************************/
void TSI_Init( void )
{
	TSI_VaribleInit();	

	// open clock gate for TSI
	SIM_SCGC5 |= SIM_SCGC5_TSI_MASK;
	// enable TSI0 and TSI1 as touch sensor PAD
	
	TSI0_TSHD = TSI_TSHD_THRESH(TSI_HIGH_THRESHOLD)|
				TSI_TSHD_THRESL(TSI_LOW_THRESHOLD);

	TSI0_GENCS = TSI_GENCS_NSCN(TSI_CS1_NSCN_16)|
				 TSI_GENCS_PS(TSI_CS1_PS_16)|
				 TSI_GENCS_ESOR_MASK|
				 TSI_GENCS_MODE(TSI_MODE_CAP_SENSE)|
				 TSI_GENCS_DVOLT(TSI_CS2_DVOLT_11)|
				 TSI_GENCS_EXTCHRG(TSI_CONSTANT_CURRENT_16uA)|
				 TSI_GENCS_REFCHRG(TSI_CONSTANT_CURRENT_16uA)|
				 TSI_GENCS_TSIIEN_MASK|			// enable interrupt
				 TSI_GENCS_STM_MASK|			// enable period scan 
				 TSI_GENCS_STPE_MASK;			// enable low power mode
    
	// enable TSI module
    TSI0_DATA = m_ElecWorkInfo[0].Channel;
	TSI0_GENCS |= TSI_GENCS_TSIEN_MASK;			// enable TSI
	enable_irq(26);								// TSI IRQ enable
				 
}

/*****************************************************************************//*!
   +FUNCTION----------------------------------------------------------------
   * @function name: TSI_Processing
   *
   * @brief process the scanning data to determine the key status
   *        
   * @param  none 
   *
   * @return none
   *
   * @ Pass/ Fail criteria: none
   *****************************************************************************/
void TSI_Processing( void )
{
	unsigned char i;
	unsigned short uiDelta;

	// determine key state
	for(i=0;i<ELECTRODE_NUM;i++)
	{
		if( m_ElecWorkInfo[i].Delta > 0 )
		{
			uiDelta = m_ElecWorkInfo[i].Delta;
		}
		else
		{
			uiDelta = 0;
		}
		if( uiDelta > KEY_ON_SENSITIVITY )
		{
			if( m_ElecWorkInfo[i].State == KEY_RELEASED )
			{
				if(m_ElecWorkInfo[i].Count > KEY_OVERTIME)
				{
					m_ElecWorkInfo[i].State = KEY_TOUCHED;
					m_ucSignalChangeFlag |= (0x01<<i);
					m_ElecWorkInfo[i].Count = 0;
				}
				else
				{
					m_ElecWorkInfo[i].Count ++;
				}
			}
			else if( m_ElecWorkInfo[i].State == KEY_TOUCHED )
			{
				//repeat touch
				//
			}
		}
		else if( uiDelta < KEY_OFF_SENSITIVITY )
		{
			if( m_ElecWorkInfo[i].State == KEY_TOUCHED )
			{
				if(m_ElecWorkInfo[i].Count > KEY_OVERTIME)
				{
					m_ElecWorkInfo[i].State = KEY_RELEASED;
					m_ucSignalChangeFlag |= (0x01<<i);
					m_ElecWorkInfo[i].Count = 0;
				}
				else
				{
					m_ElecWorkInfo[i].Count ++;
				}
			}
			else if( m_ElecWorkInfo[i].State == KEY_RELEASED )
			{
				// always released
				//
			}
		}
		else
		{
			// is noise or other
			//
		}
	}
	// check key state,processing
	TSI_CheckState();
}
/*****************************************************************************//*!
   +FUNCTION----------------------------------------------------------------
   * @function name: TSI_CheckState
   *
   * @brief check the change of the key status and implement the corresponding function
   *        
   * @param  none 
   *
   * @return none
   *
   * @ Pass/ Fail criteria: none
   *****************************************************************************/
void TSI_CheckState( void )
{
	unsigned char i;
	if( m_ucSignalChangeFlag != 0 )
	{
		// update data idle time 
		m_ulIdleTimeCount = g_ulCurrentTimeCount;
		//key state occur change,handle corresponding key
		for(i=0;i<ELECTRODE_NUM;i++)
		{
			if( m_ucSignalChangeFlag&(0x01<<i) )
			{
				//key 0 release or touched
				if( m_ElecWorkInfo[i].State == KEY_TOUCHED)
				{
					//
					KeyHandle[i]();
				}
				//clear flag
				m_ucSignalChangeFlag &= (~(0x01<<i)); 
			}
		}
	}
	else
	{
		// no signal change occur
		if( (g_ulCurrentTimeCount - m_ulIdleTimeCount) >= TSI_IDLE_TIME )
		{
       
			printf("KL05 will enter into stop mode!\n");
			printf("if long touch 'D8' will wake up KL05!\n");
			m_ulIdleTimeCount = g_ulCurrentTimeCount;
			while( TSI0_GENCS &TSI_GENCS_SCNIP_MASK );
			g_KeyCheckMode = KEY_CHECK_MODE_STANDBY;
			lptmr_init(PERIOD_LP_SCAN_MODULO,1);
            
            		// clear out of range flag
           	TSI0_GENCS |= TSI_GENCS_OUTRGF_MASK;
			// enable out of range interrupt
			//TSI0_GENCS &= (~TSI_GENCS_ESOR_MASK);
            // enable channel 0
            TSI0_DATA =  TSI_DATA_TSICH(m_ElecWorkInfo[0].Channel);
			// enable hwadware trigger
			TSI0_GENCS |= (TSI_GENCS_STM_MASK);
         	DisableOthersInterrupt();
			m_bOutOfRangeFlag = 0;
			while( !m_bOutOfRangeFlag )
			{
				stop();
			}
			EnableOthersInterrupt();
			printf("KL05 restore from stop mode!\n");

		}	
	}
}

/*****************************************************************************//*!
   +FUNCTION----------------------------------------------------------------
   * @function name: TSI_ISR
   *
   * @brief TSI interrupt service routine 
   *        
   * @param  none 
   *
   * @return none
   *
   * @ Pass/ Fail criteria: none
   *****************************************************************************/
void TSI_isr ( void )
{
    uint32_t Gencs_Register;
    Gencs_Register = TSI0_GENCS;
   	if( Gencs_Register & TSI_GENCS_OUTRGF_MASK )
	{
        // printf("TSI out of range!\n");
		// this is only set in low power
		TSI0_GENCS |= TSI_GENCS_OUTRGF_MASK;
		m_bOutOfRangeFlag = 1;
		if( g_KeyCheckMode != KEY_CHECK_MODE_NORMAL )
		{
			g_KeyCheckMode = KEY_CHECK_MODE_NORMAL;
			TSI0_GENCS |= TSI_GENCS_ESOR_MASK;
			// switch software trigger mode
			TSI0_GENCS &= (~TSI_GENCS_STM_MASK);
			// trigger STI scanning
			m_ucScanningIndex = 0;
			TSI0_DATA =  TSI_DATA_TSICH(m_ElecWorkInfo[m_ucScanningIndex].Channel);
                        TSI0_DATA |= TSI_DATA_SWTS_MASK;
			// modify LPTMR to noraml scanning frequency
			lptmr_init(PERIOD_NORMAL_SCAN_MODULO,1);
		}
	}
	
	if( Gencs_Register & TSI_GENCS_EOSF_MASK )
	{
        // printf(" TSI eof !\n");
		TSI0_GENCS |= TSI_GENCS_EOSF_MASK;
        if( g_KeyCheckMode == KEY_CHECK_MODE_NORMAL )
        {
          // switch software trigger mode
          TSI0_GENCS &= (~TSI_GENCS_STM_MASK);
          if( m_ucScanningIndex < ELECTRODE_NUM )
          {
              m_ElecWorkInfo[m_ucScanningIndex++].Samples = (uint16_t)TSI0_DATA;
              TSI0_DATA =  TSI_DATA_TSICH(m_ElecWorkInfo[m_ucScanningIndex].Channel);
                          TSI0_DATA |= TSI_DATA_SWTS_MASK;
          }
          else
          {
              //all of channels scan completed,setting flag
              m_bScanCompleteFlag = 1;
              // counter clear 0
              m_ucScanningIndex = 0;
              // switch to harware trigger
              TSI0_GENCS |= (TSI_GENCS_STM_MASK);
          }
        }
		
	}
}


/*****************************************************************************//*!
   +FUNCTION----------------------------------------------------------------
   * @function name: TSI_Check
   *
   * @brief check TSI scan state and process touched action
   *        when on touch last some times,make MCU enter stop mode.
   *        when check there are touch action occur, restore to normal mode
   * 
   *        
   * @param  none 
   *
   * @return none
   *
   * @ Pass/ Fail criteria: none
   *****************************************************************************/
void TSI_Check( void )
{
	unsigned char i;

	if( !m_bScanCompleteFlag )
	{
		return;
	}
	m_bScanCompleteFlag = false;
	if( m_ucTSI_WorkState != TSI_WORK_STATE_NORMAL )
	{
		for(i=0;i<ELECTRODE_NUM;i++)
		{
			if( m_ucTSI_WorkState == TSI_WORK_STATE_NULL )
			{
				m_ElecWorkInfo[i].BaseLine = m_ElecWorkInfo[i].Samples;
			}
			else
			{
				m_ElecWorkInfo[i].BaseLine = (m_ElecWorkInfo[i].Samples + m_ElecWorkInfo[i].BaseLine)>>1;
			}
		}
		m_ucTSI_WorkState ++;
		if( m_ucTSI_WorkState > 3)
		{
			m_ucTSI_WorkState = TSI_WORK_STATE_NORMAL;
			
			m_ulIdleTimeCount = g_ulCurrentTimeCount;
		}
		else
		{
			return;
		}
	}
		
	for(i=0;i<ELECTRODE_NUM;i++)
	{
		m_ElecWorkInfo[i].Delta = m_ElecWorkInfo[i].Samples - m_ElecWorkInfo[i].BaseLine;
	}
	TSI_Processing();
}
// this function only for test
void TSI_SynTime( void )
{
	m_ulIdleTimeCount = g_ulCurrentTimeCount;
}

void EnableOthersInterrupt( void )
{
	enable_irq(46 -16);
	enable_irq(47 -16);
	enable_irq(34 -16);
	enable_irq(36 -16);
	enable_irq(38 -16);
}
void DisableOthersInterrupt( void )
{
	disable_irq(46 -16);
	disable_irq(47 -16);
	disable_irq(34 -16);
	disable_irq(36 -16);
	disable_irq(38 -16);
}



	




