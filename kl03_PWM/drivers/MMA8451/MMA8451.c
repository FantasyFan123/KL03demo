/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2011-2012 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file mma8451.c
*
* @author B37811
*
* @version 0.0.1
*
* @date Sep 19, 2011
*
* @brief application entry point which performs application specific tasks. 
*
*******************************************************************************
*
* provide a demo for how to initialize the PT60, output messages via SCI, 
* flash operations, etc.
* NOTE:
*	printf call may occupy a lot of memory (around 1924 bytes), so please
*	consider your code size before using printf.
******************************************************************************/
#include "MMA8451.h"
#include "iic.h"
#include "tower.h"

void MMA8451_Standby(void);
void MMA8451_Active(void);
void MMA8451WriteRegister(uint8 address, uint8 reg,uint8 val);
uint8 u8MMA8451ReadRegister(uint8 address, uint8 reg);
void MMA8451WriteRegister_N(uint8 address, uint8 reg1,uint8 N,uint8 *array);
void MMA8451ReadRegister_N(uint8 address, uint8 reg1,uint8 N,uint8 *array);
void MMA8451_Read_3axis_data(void);

void MMA8451_Initial(void);

uint16  resultx, resulty, resultz;

/*****************************************************************************//*!
   +FUNCTION----------------------------------------------------------------
   * @function name: MMA8451_Standby
   *
   * @brief description: configure the MMA8451 as standby mode
   *        
   * @parameter:  none
   *
   * @return: none
   *
   * @ Pass/ Fail criteria: none
   *****************************************************************************/
void MMA8451_Standby(void)
{
/*
** Read current value of System Control 1 Register.
** Put sensor into Standby Mode by clearing the Active bit
** Return with previous value of System Control 1 Register.
*/
    MMA8451WriteRegister(MMA8451_I2C_ADDRESS, CTRL_REG1, u8MMA8451ReadRegister(MMA8451_I2C_ADDRESS,CTRL_REG1) & ~ACTIVE_MASK);
}


/*****************************************************************************//*!
   +FUNCTION----------------------------------------------------------------
   * @function name: MMA8451_Active
   *
   * @brief description: configure the MMA8451 as Active mode
   *        
   * @parameter:  none
   *
   * @return: none
   *
   * @ Pass/ Fail criteria: none
   *****************************************************************************/
void MMA8451_Active(void)
{
/*
** Set the Active bit in CTRL Reg 1
*/
    MMA8451WriteRegister(MMA8451_I2C_ADDRESS, CTRL_REG1, (u8MMA8451ReadRegister(MMA8451_I2C_ADDRESS, CTRL_REG1) | ACTIVE_MASK));
}

/*********************************************************\
* I2C Write Register
\*********************************************************/


void MMA8451WriteRegister(uint8 address, uint8 reg,uint8 val)
{
  //I2C0_C1_TX = 1;                                // Transmit Mode
  I2C0_C1 |= I2C_C1_TX_MASK; 
  I2C_Start();                                  // Send Start
  I2C_CycleWrite(address);                      // Send I2C "Write" Address
  I2C_CycleWrite(reg);                          // Send Register
  I2C_CycleWrite(val);                          // Send Value
  I2C_Stop();                                   // Send Stop
}


/*********************************************************\
* I2C Read Register
\*********************************************************/
uint8 u8MMA8451ReadRegister(uint8 address, uint8 reg)
{
  uint8 b;
  //I2C_C1_TX = 1;                                // Transmit Mode
  I2C0_C1 |= I2C_C1_TX_MASK; 
  I2C_Start();                                  // Send Start
  I2C_CycleWrite(address);                      // Send I2C "Write" Address
  I2C_CycleWrite(reg);                          // Send Register
  I2C_RepeatStart();                            // Send Repeat Start
  I2C_CycleWrite(address+1);                    // Send I2C "Read" Address
  b = I2C_CycleRead(1);                         // *** Dummy read: reads "I2C_ReadAddress" value ***
  b = I2C_CycleRead(1);                         // Read Register Value
  I2C_Stop();                                   // Send Stop
  return b;
}


/*********************************************************\
* I2C Write Multiple Registers
\*********************************************************/
void MMA8451WriteRegister_N(uint8 address, uint8 reg1,uint8 N,uint8 *array)
{
  //I2C_C1_TX = 1;                                // Transmit Mode
  I2C0_C1 |= I2C_C1_TX_MASK; 
  I2C_Start();                                  // Send Start
  I2C_CycleWrite(address);                      // Send I2C "Write" Address
  I2C_CycleWrite(reg1);                         // Send Register
  while (N>0)                                   // Send N Values
  {
    I2C_CycleWrite(*array);
    array++;
    N--;
  }
  I2C_Stop();                                   // Send Stop
}


/*********************************************************\
* I2C Read Multiple Registers
\*********************************************************/
void MMA8451ReadRegister_N(uint8 address, uint8 reg1,uint8 N,uint8 *array)
{
  uint8 b;
  //I2C_C1_TX = 1;                                // Transmit Mode
  I2C0_C1 |= I2C_C1_TX_MASK; 
  I2C_Start();                                  // Send Start
  I2C_CycleWrite(address);                      // Send I2C "Write" Address
  I2C_CycleWrite(reg1);                         // Send Register
  I2C_RepeatStart();                            // Send Repeat Start
  I2C_CycleWrite(address+1);                    // Send I2C "Read" Address
  b = I2C_CycleRead(0);                         // *** Dummy read: reads "I2C_ReadAddress" value ***
  while (N>1)                                   // Read N-1 Register Values
  {
    b = I2C_CycleRead(0);
    *array = b;
    array++;
    N--;
  }
  b = I2C_CycleRead(1);
  *array = b;                                   // Read Last value
  I2C_Stop();                                   // Send Stop
}


/*****************************************************************************//*!
   +FUNCTION----------------------------------------------------------------
   * @function name: MMA8451_Read_3axis_data
   *
   * @brief description: Read first three registers from the MMA8451
   *        
   * @parameter:  none
   *
   * @return: none
   *
   * @ Pass/ Fail criteria: none
   *****************************************************************************/
void MMA8451_Read_3axis_data(void)
{
   // if((u8MMA8451ReadRegister(MMA8451_I2C_ADDRESS,F_STATUS_REG) & 0x0f) != 0)  // polling
    {
        resultx   = u8MMA8451ReadRegister(MMA8451_I2C_ADDRESS, 0x01)<<8;
        resultx  |= u8MMA8451ReadRegister(MMA8451_I2C_ADDRESS, 0x02);
        resultx >>= 2;

        resulty   = u8MMA8451ReadRegister(MMA8451_I2C_ADDRESS, 0x03)<<8;
        resulty  |= u8MMA8451ReadRegister(MMA8451_I2C_ADDRESS, 0x04);
        resulty >>= 2;

        resultz   = u8MMA8451ReadRegister(MMA8451_I2C_ADDRESS, 0x05)<<8;
        resultz  |= u8MMA8451ReadRegister(MMA8451_I2C_ADDRESS, 0x06);
        resultz >>= 2;
    }
}

/*****************************************************************************//*!
   +FUNCTION----------------------------------------------------------------
   * @functionDATA_STATUS_REG name: MMA8451_Initial
   *
   * @brief description: MMA8451 Initial as  Tap Detection mode.
   *        
   * @parameter:  none
   *
   * @return: none
   *
   * @ Pass/ Fail criteria: none
   *****************************************************************************/
void MMA8451_Initial(void)
{
    uint8    n, int_mask;
    uint8    CurrentReadMode;
    uint8    CurrentRange;
    uint8    CurrentDemo;
		
    CurrentReadMode = READ_MODE_14BITS;
    CurrentRange    = Full_Scale_Range_2g;
    CurrentDemo     = DEMO_TAP;
	
    /* Put MMA845xQ into Standby Mode */	
	MMA8451_Standby();  
    
    /* Configure sensor for:
         Sleep Mode Poll Rate of 1.56Hz (640ms) for maximum power saving 
         System Output Data Rate (ODR) of 200Hz (5ms)
		 read mode 14-bit data
    */
//    n = ASLP_RATE_640MS + DATA_RATE_5MS;   
    n = ASLP_RATE_20MS + DATA_RATE_5MS;  
    if (CurrentReadMode == READ_MODE_8BITS)
        n |= FREAD_MASK;
    MMA8451WriteRegister(MMA8451_I2C_ADDRESS, CTRL_REG1, n);  
  
    /* Select user-specified sensitivity, 2g/4g/8g */
    MMA8451WriteRegister(MMA8451_I2C_ADDRESS, XYZ_DATA_CFG_REG, CurrentRange);
    
    /* Configure Sleep/Wake modes 
        SMODS1:0   : 1 1    Low power mode when asleep
        SLPE       : 1      Sleep enabled
        MODS1:0    : 0 0    Normal mode when awake
    */
    MMA8451WriteRegister(MMA8451_I2C_ADDRESS, CTRL_REG2, 0x1c);          
    MMA8451WriteRegister(MMA8451_I2C_ADDRESS, ASLP_COUNT_REG, 15);      /* Sleep after 10 seconds of inactivity */         

    /* Disable the FIFO */
    MMA8451WriteRegister(MMA8451_I2C_ADDRESS, F_SETUP_REG, 0x00);

    switch (CurrentDemo) 
    {
    case DEMO_TRANSIENT:
    case DEMO_SHAKE:
    case DEMO_ORIENTATION:
        /* In all three of these demo modes we configure the accelerometer to detect
           movement:
           
                DEMO_TRANSIENT   - We configure the accelerometer to detect small movements
                                   of > 0.063g                   
                                   
                DEMO_SHAKE       - We configure the accelerometer to detect movements
                                   of > 0.5g
                                                      
                DEMO_ORIENTATION - We don't care about the movement data itself, but
                                   we use transient detection so that the accelerometer
                                   can tell us when the board isn't being used. When
                                   it transitions to Sleep mode, we can put the main
                                   processor to sleep too.
                                   
          By using the high-pass filter we can remove the constant effect of gravity,
           and only detect changes in acceleration. See Application Note AN4071.
        */

        /* ELE = 1     : Event latch enabled 
           ZTEFE = 1   : Raise event on any of Z, Y, X axes
           YTEFE = 1
           XTEFE = 1
           HBF_BYP = 0 : High-pass filter enabled
        */
    	MMA8451WriteRegister(MMA8451_I2C_ADDRESS, TRANSIENT_CFG_REG, 0x1e);
        
        /* Set High-pass filter cut-off frequency for best sensitivity */
    	MMA8451WriteRegister(MMA8451_I2C_ADDRESS, HP_FILTER_CUTOFF_REG, 0x03);   

        if (CurrentDemo == DEMO_SHAKE) 
        {
            /* Transient is indicated when acceleration on one of the axes
               is above threshold 8 x 0.063g = 0.5g 
             */   
        	MMA8451WriteRegister(MMA8451_I2C_ADDRESS, TRANSIENT_THS_REG, 8);
        } 
        else 
        {
            /* Transient is indicated when acceleration on one of the axes
               is above threshold 1 x 0.063g - i.e. a small movement 
            */   
        	MMA8451WriteRegister(MMA8451_I2C_ADDRESS, TRANSIENT_THS_REG, 1);
        }

        /* Internal debounce counter. For an ODR of 200Hz in Normal mode,
           motion is indicated after 5 x 1 = 5ms. 
        */
         MMA8451WriteRegister(MMA8451_I2C_ADDRESS, TRANSIENT_COUNT_REG, 1);

        /* Interrupt signalled on INT1 when transient detected */
        int_mask = INT_EN_TRANS_MASK;

        if (CurrentDemo == DEMO_ORIENTATION) 
        {
            /* Interrupt also signalled when new data is ready */
             int_mask |= INT_EN_DRDY_MASK;
         
            /* Set up TPMs to produce edge-aligned PWM signals */
            //configure_LEDs_for_PWM ();    
        }
        break;
        
    case DEMO_TAP:
        /* Configure the accelerometer to detect single and double taps... 
           (See Application Note AN4072)
        */
 
        /* Z configured for Single Tap and Double Tap with Latch enabled */
    	MMA8451WriteRegister(MMA8451_I2C_ADDRESS, PULSE_CFG_REG, 0x70);    
        
        /* Enable low-pass filter */
    	MMA8451WriteRegister(MMA8451_I2C_ADDRESS, HP_FILTER_CUTOFF_REG, PULSE_LPF_EN_MASK);   

        /* Set Z Threshold to 16 x 0.063g = 1.0g 
           
           Note: In a more sophisticated application we could dynamically
           track the orientation and configure X, Y Z with changing thresholds
        */
    	MMA8451WriteRegister(MMA8451_I2C_ADDRESS, PULSE_THSZ_REG, 16);  

        /* Set the Pulse Time Limit for 30 ms at 200 Hz ODR in Normal Mode with the LPF Enabled  
           30 ms/5 ms = 6 counts
        */
    	MMA8451WriteRegister(MMA8451_I2C_ADDRESS, PULSE_TMLT_REG, 6);
        
        /* Set the Pulse Latency Timer to 100 ms, 200 Hz ODR Normal Mode, LPF Enabled  
           80 ms/10 ms = 8 counts
        */
    	MMA8451WriteRegister(MMA8451_I2C_ADDRESS, PULSE_LTCY_REG, 8);
        
        /* Set the Pulse window to 400 ms, 200 Hz Normal Mode, LPF Enabled
           300 ms/10 ms = 30 counts
        */
    	MMA8451WriteRegister(MMA8451_I2C_ADDRESS, PULSE_WIND_REG, 30);

        /* Interrupt signalled on INT1 when pulse detected */
        int_mask = INT_EN_PULSE_MASK;
        break;
        
    case DEMO_FREEFALL:
        /* Configure accelerometer for linear freefall detection... 
           [Note that the accelerometer can also detect tumbling, as
            described in Application Note AN4070]
        */
        
        /* ELE = 1   : Event latch enabled 
           OAE = 0   : Detect Freefall
           ZEFE = 1  : Raise event on any of Z, Y, X axes
           YEFE = 1
           XEFE = 1
        */
    	MMA8451WriteRegister(MMA8451_I2C_ADDRESS, FF_MT_CFG_1_REG, 0xb8);   
        
        /* Freefall is indicated when acceleration on all three axes
           falls below threshold 3 x 0.063g = 0.19g 
        */   
    	MMA8451WriteRegister(MMA8451_I2C_ADDRESS, FT_MT_THS_1_REG, 3);
        
        /* Internal debounce counter. For an ODR of 200Hz in Normal mode,
           freefall is indicated after 5 x 20 = 100ms of falling - a drop
           of a few centimetres - See Application Note AN4070
        */
    	MMA8451WriteRegister(MMA8451_I2C_ADDRESS, FF_MT_COUNT_1_REG, 20);

        /* Interrupt signalled on INT1 when Freefall detected */
        int_mask = INT_EN_FF_MT_1_MASK;
        break;
    }
   
    /* Configure interrupts */
    int_mask |= INT_EN_ASLP_MASK;                           /* Also generate interrupt on 
                                                               Sleep <--> Wake transition 
                                                            */
    MMA8451WriteRegister(MMA8451_I2C_ADDRESS, CTRL_REG4, int_mask);
    MMA8451WriteRegister(MMA8451_I2C_ADDRESS, CTRL_REG5, 0xfd);         /* All interrupts mapped to MMA_INT1 */
    MMA8451WriteRegister(MMA8451_I2C_ADDRESS, CTRL_REG3, 0x78);         /* Active-low interrupts, all functions 
                                                                           wake system 
                                                                        */  

    /* Throw away any stale interrupt info */
//    last_int_source_reg = 0;
    
    /* Put MMA845xQ into Active Mode */
	MMA8451_Active();

    /* Enable MMA_INTx interrupts */
//    KBI1_SC_KBIE  = 1;  
		 
}

/*
 * end here
 */
