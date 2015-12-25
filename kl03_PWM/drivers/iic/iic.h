/*
 * File:		iic.h
 * Purpose:     
 *
 * Notes:
 */

#ifndef __IIC_H__
#define __IIC_H__

#include "common.h"

/********************************************************************/

#define i2c_DisableAck()       I2C0_C1 |= I2C0_C1_TXAK_MASK

#define i2c_SendAck()          I2C0_SMB |= I2C0_SMB_FACK_MASK; \
	                           I2C0_C1  &= ~I2C0_C1_TXAK_MASK     

#define i2c_RepeatedStart()    I2C0_C1 |= I2C0_C1_RSTA_MASK;

#define i2c_Start()            I2C0_C1 |= I2C0_C1_TX_MASK;\
                               I2C0_C1 |= I2C0_C1_MST_MASK;\
                               while((!(I2C0_S & I2C0_S_BUSY_MASK))       // wait until busy

#define i2c_Stop()             I2C0_C1 &= ~I2C0_C1_MST_MASK;\
                               I2C0_C1 &= ~I2C0_C1_TX_MASK;\
                               while(I2C0_S & I2C0_S_BUSY_MASK)        // wait until not busy

#define i2c_EnterRxMode()      I2C0_C1 &= ~I2C0_C1_TX_MASK;\
                               I2C0_C1 &= ~I2C0_C1_TXAK_MASK

#define i2c_Wait()             while((I2C0_S & I2C0_S_IICIF_MASK)==0); \
                               I2C0_S |= I2C0_S_IICIF_MASK

#define i2c_write_byte(data)   I2C0_D = data


extern void Init_I2C(void);
extern void I2C_Start(void);
extern void I2C_Stop(void);
extern void I2C_RepeatStart(void);
extern void I2C_Delay(void);
extern void I2C_CycleWrite(uint8 bout);
extern uint8 I2C_CycleRead(uint8 ack);

/********************************************************************/

#endif /* __IIC_H__ */
