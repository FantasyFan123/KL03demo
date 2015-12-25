

#include "common.h"
#include "hal_i2c.h"
#include "hal_dev_mma8451.h"



void hal_dev_mma8451_init(void)
{
  hal_i2c_init(I2C0_B);
}




// this delay is very important, it may cause w-r operation failure.
static void pause(void)
{
    int n;
    for(n=0; n<40; n++)
        __nop();
}
uint8 hal_dev_mma8451_read_reg(uint8 addr)
{
    uint8 result;
    int i;  
	
    i2c_start(I2C0_B);
    i2c_write_byte(I2C0_B, MMA8451_I2C_ADDRESS | I2C_WRITE);
    
    i2c_wait(I2C0_B);
    i2c_get_ack(I2C0_B);

    i2c_write_byte(I2C0_B, addr);
    i2c_wait(I2C0_B);
    i2c_get_ack(I2C0_B);

    i2c_repeated_start(I2C0_B);
		for(i=0;i<1000;i++)
       for( i=0;i<1000;i++);
	
    i2c_write_byte(I2C0_B, MMA8451_I2C_ADDRESS | I2C_READ);
    i2c_wait(I2C0_B);
    i2c_get_ack(I2C0_B);

    i2c_set_rx_mode(I2C0_B);

    i2c_give_nack(I2C0_B);
    result = i2c_read_byte(I2C0_B);
    i2c_wait(I2C0_B);

    i2c_stop(I2C0_B);
    result = i2c_read_byte(I2C0_B);
    pause();
		for(i=0;i<1000;i++)
       for( i=0;i<1000;i++);
    return result;
}
void hal_dev_mma8451_write_reg(uint8 addr, uint8 data)
{
		int i; 
    i2c_start(I2C0_B);

    i2c_write_byte(I2C0_B, MMA8451_I2C_ADDRESS|I2C_WRITE);
    i2c_wait(I2C0_B);
    i2c_get_ack(I2C0_B);

    i2c_write_byte(I2C0_B, addr);
    i2c_wait(I2C0_B);
    i2c_get_ack(I2C0_B);

    i2c_write_byte(I2C0_B, data);
    i2c_wait(I2C0_B);
    i2c_get_ack(I2C0_B);

    i2c_stop(I2C0_B);
    pause();
		for(i=0;i<1000;i++)
		 for( i=0;i<1000;i++);
}





