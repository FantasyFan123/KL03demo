/*
 * File:        iic.c
 * Purpose:     Provide common adc routines
 *
 * Notes:       
 *              
 */

#include "common.h"
#include "iic.h"


//#include "common.h" /* for EnableInterrupts macro */
//#include "MK15E7.h" /* include peripheral declarations */
//#include "I2C.h"

//#pragma MESSAGE DISABLE C4002 /* Warning C4002: Result not used */  


/***********************************************************************************************\
* Private prototypes
\***********************************************************************************************/

void I2C_Start(void);
void I2C_Stop(void);
void I2C_RepeatStart(void);
void I2C_Delay(void);
void I2C_CycleWrite(uint8 bout);
uint8 I2C_CycleRead(uint8 ack);
void Init_I2C(void);

/***********************************************************************************************\
* Private memory declarations
\***********************************************************************************************/

//#pragma DATA_SEG __SHORT_SEG _DATA_ZEROPAGE

static uint8 error;
static uint16 timeout;

//#pragma DATA_SEG DEFAULT

#define BUFFER_OUT_SIZE       8


/*****************************************************************************//*!
   +FUNCTION----------------------------------------------------------------
   * @function name: init_I2C
   *
   * @brief description: I2C Initialization, Set Baud Rate and turn on I2C
   *        
   * @parameter:  none 
   *
   * @return: none
   *
   * @ Pass/ Fail criteria: none
   *****************************************************************************/
void Init_I2C(void)
{
    /* I2C1 module is used in KE15 tower */	

    SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK; // enable PORTA clock
    PORTB_PCR3 = PORT_PCR_MUX(2);
    PORTB_PCR4 = PORT_PCR_MUX(2);
	
    SIM_SCGC4 |= SIM_SCGC4_I2C0_MASK;  // enable IIC0 clock
    I2C0_F  = 0x1F;   // about 100K baud rate
    I2C0_C1 = I2C_C1_IICEN_MASK |    // enable IIC 
              //I2C0_C1_IICIE_MASK |    
              I2C_C1_MST_MASK   |    // master
              I2C_C1_TX_MASK;           
}



/***********************************************************************************************\
* Private functions
\***********************************************************************************************/

/*********************************************************\
* Initiate I2C Start Condition
\*********************************************************/
void I2C_Start(void)
{
  I2C0_C1 |= I2C_C1_MST_MASK;
  timeout = 0;
  while ((!(I2C0_S & I2C_S_BUSY_MASK)) && (timeout<10000))
    timeout++;
  if (timeout >= 10000)
    error |= 0x01;
} //*** Wait until BUSY=1


/*********************************************************\
* Initiate I2C Stop Condition
\*********************************************************/
void I2C_Stop(void)
{
  I2C0_C1 &= ~I2C_C1_MST_MASK;
  timeout = 0;
  while ( (I2C0_S & I2C_S_BUSY_MASK) && (timeout<10000))
    timeout++;
  if (timeout >= 10000)
    error |= 0x02;
} //*** Wait until BUSY=0


/*********************************************************\
* Initiate I2C Repeat Start Condition
\*********************************************************/
void I2C_RepeatStart(void)
{
	
  I2C0_C1 |= I2C_C1_RSTA_MASK;
  timeout = 0;
  while ((!(I2C0_S & I2C_S_BUSY_MASK)) && (timeout<10000))
    timeout++;
  if (timeout >= 10000)
    error |= 0x04;
} //*** Wait until BUSY=1


/*********************************************************\
* I2C Delay
\*********************************************************/
void I2C_Delay(void)
{
  uint8 I2Cd;
  for (I2Cd=0; I2Cd<100; I2Cd++);
}


/*********************************************************\
* I2C Cycle Write
\*********************************************************/
void I2C_CycleWrite(uint8 bout)
{
  timeout = 0; 
  while ((!(I2C0_S & I2C_S_TCF_MASK)) && (timeout<10000))
    timeout++;
  if (timeout >= 10000)
    error |= 0x08; 
  I2C0_D = bout; 
  timeout = 0;
  while ((!(I2C0_S & I2C_S_IICIF_MASK)) && (timeout<10000))
    timeout++;
  if (timeout >= 10000)
    error |= 0x10;
  I2C0_S &= I2C_S_IICIF_MASK;    // clear the int pending flag
  if (I2C0_S & I2C_S_RXAK_MASK)  // no ack received
    error |= 0x20;
}


/*********************************************************\
* I2C Cycle Read
\*********************************************************/
uint8 I2C_CycleRead(uint8 ack)
{
  uint8 bread; 
  timeout = 0;
  while ((!(I2C0_S & I2C_S_TCF_MASK)) && (timeout<10000))
    timeout++;
  if (timeout >= 10000)
    error|=0x08;
  I2C0_C1 &= ~I2C_C1_TX_MASK;     // Receive mode   
  I2C0_C1 |= ((ack << 3) & 0x8);    // TXAK
  bread = I2C0_D; 
  timeout = 0; 
  while ((!(I2C0_S & I2C_S_IICIF_MASK)) && (timeout<10000))
    timeout++;
  if (timeout >= 10000)
    error |= 0x10;
  I2C0_S &= I2C_S_IICIF_MASK;    // clear the int pending flag

  return bread;
}

/*
 * end here
 */
