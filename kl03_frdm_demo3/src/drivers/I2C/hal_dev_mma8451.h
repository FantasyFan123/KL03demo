
#define MMA8451_I2C_ADDRESS (0x1d<<1)
#define I2C0_B  I2C0_BASE_PTR

void hal_dev_mma8451_init(void); //prepare I2C port
uint8   hal_dev_mma8451_read_reg(uint8 addr);
void hal_dev_mma8451_write_reg(uint8 addr, uint8 data);
uint16 hal_dev_mma8451_test_reg_rw(void);
void hal_dev_mma8451_test_function(void);

