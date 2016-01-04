/*
 * File:		platinum.c
 * Purpose:		Main process
 *
 */

#include "common.h"
#include "MKL_gpio.h"
#include "uart.h"
#include "lptmr.h"
#include "hal_dev_mma8451.h"
#include "smc.h"

uint8 data;
uint8 flag_500ms;
extern int uart0_clk_khz;
extern uint8 error;
//signed short accel_x, accel_y, accel_z;
//signed short resultx, resulty, resultz;

uint8 WHO_AM_I;
uint8 data;
/********************************************************************/
#define MCGIRCLK		0
#define LPO					1
#define ERCLK32K		2
#define OSCERCLK		3
/********************************************************************/

void SPI0_init(void);
void TPM_init(void);

int main (void)
{
	
	uint8 tmp;
	SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK
							| SIM_SCGC5_PORTB_MASK );
	SIM_SCGC6 |= SIM_SCGC6_RTC_MASK;
	tmp = RTC_TSR;
	RTC_TSR = tmp;
	SIM_SCGC6 &= ~SIM_SCGC6_RTC_MASK;
	
	PORT_ENABLE_CLK(MKL_PORTB);  								/*使能时钟*/    
	IO_FUN_SEL(MKL_PORTB,10,1);									/*IO功能选择*/    
	GPIO_DDR_OUTPUT(MKL_PORTB,10); 							/*设置为输出模式*/    
	GPIO_CLR(MKL_PORTB,10);											/*设置为低电平，红灯亮*/
	
	
	IO_FUN_SEL(MKL_PORTB,5,1);									/*IO功能选择*/    
	GPIO_DDR_INPUT(MKL_PORTB,0); 								/*设置为输入模式*/    

	IO_IRQ_CONFIG(MKL_PORTB,5,0x0A);						/*PTB5 SW3 下降沿触发中断*/
	enable_irq(INT_PORTB - 16);									/*使能按键中断*/
	
	SYST_CSR = 0x00;
	SYST_RVR = 48000;														/*使用内核时钟48MHz 每1ms翻转一次*/
	SYST_CVR = 0;
	SYST_CSR |= SysTick_CSR_ENABLE_MASK
							| SysTick_CSR_TICKINT_MASK
							| SysTick_CSR_CLKSOURCE_MASK;
							
	IO_FUN_SEL(MKL_PORTB,11,1);									/*初始化PTB11,绿灯用于测试systick*/ 
	GPIO_DDR_OUTPUT(MKL_PORTB,11); 							  									
	
	
	enable_irq(INT_UART0 - 16);															/*使能LPUART0中断, LPUART0初始化在sysinit()中*/
	
	lptmr_init(500,LPO);												/*初始化LPTMR, 使用LPO 1kHz  定时周期500ms*/
	SPI0_init();																/*初始化SPI模块*/

	
	hal_dev_mma8451_init();
	tmp = hal_dev_mma8451_read_reg(0x0d);
	//hal_dev_mma8451_write_reg(MMA8451_I2C_ADDRESS,tmp|0x01);
	//tmp = hal_dev_mma8451_read_reg(0x02);
	printf("tmp = %x\r\n", tmp);
	
	//lptmr_init(10000,LPO);												/*初始化LPTMR, 使用LPO 1kHz  定时周期500ms*/
	//DisableInterrupts;
	//enter_vlps();
	
	TPM_init();
	while(1)
	{
		if(flag_500ms)
		{
			flag_500ms = 0;
			//out_char(data);
			while((SPI0_S & SPI_S_SPTEF_MASK) != SPI_S_SPTEF_MASK);			/*发送缓冲区为空时写SPI0_D发送数据*/
			SPI0_D = 0x0A;																							/*输出跟输入串起来测试*/
			data = SPI0_D;
			
			//accel_read();
			//printf("\nresultx = %d",resultx);
			//printf("\nresulty = %d",resulty);
			//printf("\nresultz = %d",resultz);
			//printf("\naccel_x = %d",accel_x);
			//printf("\naccel_y = %d",accel_y);
			//printf("\naccel_z = %d",accel_z);
		}		
	} 	
}


void SPI0_init(void)
{
    SIM_SCGC4 |= SIM_SCGC4_SPI0_MASK;   //open SPI0 gate clock
    SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;  //open PORTA gate clock
    
    PORTA_PCR9 |= PORT_PCR_MUX(3);      //set PTA9 as the SPI0_SCK
    PORTA_PCR8 |= PORT_PCR_MUX(3);      //set PTA8 as the SPI0_MOSI
    PORTA_PCR7 |= PORT_PCR_MUX(2);      //set PTA7 as the SPI0_MISO
    
    
    PORTA_PCR6 |= PORT_PCR_MUX(1);      //set PTA6 as GPIO
    GPIOA_PDDR |= (1u<<6);
    GPIOA_PDOR &= ~(1u<<6);
    SPI0_C1 = 0x04;
    
    SPI0_C1 |= SPI_C1_MSTR_MASK         //select SPI0 master mode
            |  SPI_C1_SPE_MASK          //enable SPI0
            |  SPI_C1_SSOE_MASK        // auto slave select
            |  SPI_C1_CPHA_MASK;
            
    //SPI0_C1 |= SPI_C1_SPTIE_MASK;       //enable SPI0 send int        
            
    SPI0_C2 = 0x00;
    
    SPI0_BR = 0x00;
    SPI0_BR |= SPI_BR_SPPR(0) | SPI_BR_SPR(0);  //band = 12m
    
    SPI0_S = 0x00;
    
    //enable_irq(SPI0_irq_no);   
}

void TPM_init(void)
{
		SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1);	  //48MHz as tpm clock source
		SIM_SCGC6 |= SIM_SCGC6_TPM0_MASK| SIM_SCGC6_TPM1_MASK;   //enable tpm clock

		TPM0_CNT = 0;
		TPM0_MOD = 48000;
		
		TPM0_SC |= TPM_SC_PS(7);						//should be configed before config TPM0_C0SC
		TPM0_SC = TPM_SC_TOIE_MASK|TPM_SC_CMOD(1);
		
		TPM0_C0SC = TPM_CnSC_MSB_MASK| TPM_CnSC_ELSB_MASK;
		TPM0_C0V = 0;
		
		enable_irq(INT_TPM0 - 16);	
}

void PORTB_IRQHandler(void)
{
	if(PORTB_ISFR & (1<<5))
	{
		PORTB_ISFR |= PORT_ISFR_ISF(1<<5);
		GPIO_TOGGLE(MKL_PORTB,10);
	}
}

void SysTick_Handler(void)
{
	uint8 i; 
	//GPIO_TOGGLE(MKL_PORTB,11);
	i = SYST_CSR;																/*读SYST_CSR,清COUNTFLAG*/

}

void LPUART0_IRQHandler(void)
{
	if(LPUART0_STAT & LPUART_STAT_OR_MASK)
	{
		LPUART0_STAT |= LPUART_STAT_OR_MASK;			/*写1清过载中断*/
	}
	
	if(LPUART0_STAT & LPUART_STAT_RDRF_MASK)		/*读接收数据*/
	{
		data = LPUART0_DATA;
		out_char(data);
	}
}

void LPTMR0_IRQHandler(void)
{
	//write 1 to TCF to clear the LPT timer compare flag
	LPTMR_CSR |=  LPTMR_CSR_TCF_MASK;
	flag_500ms = 1;
}


void TPM0_IRQHandler(void)
{
	if(TPM0_SC & TPM_SC_TOF_MASK)
	{
		TPM0_SC |= TPM_SC_TOF_MASK;
		GPIO_TOGGLE(MKL_PORTB,11);
	}
}
