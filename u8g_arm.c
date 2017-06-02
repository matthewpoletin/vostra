/*
	The following delay procedures must be implemented for u8glib. This is done in this file:

	void u8g_Delay(uint16_t val)		Delay by "val" milliseconds
	void u8g_MicroDelay(void)		Delay be one microsecond
	void u8g_10MicroDelay(void)	Delay by 10 microseconds
	
	Additional requirements:
	
			SysTick must be enabled, but SysTick IRQ is not required. Any LOAD values are fine,
			it is prefered to have at least 1ms
			Example:
				SysTick->LOAD = (SystemCoreClock/1000UL*(unsigned long)SYS_TICK_PERIOD_IN_MS) - 1;
	SysTick->VAL = 0;
	SysTick->CTRL = 7;   // enable, generate interrupt (SysTick_Handler), do not divide by 2
*/

#include "u8g_arm.h"

uint8_t control = 0;

/* set TIM5 to run at DELAY_TIM_FREQUENCY */
void delay_init(void)
{
	/* Enable timer clock  - use TIMER5 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	/* Time base configuration */
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = (SYS_CLK / DELAY_TIM_FREQUENCY) - 1;
	TIM_TimeBaseStructure.TIM_Period = UINT16_MAX;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	/* Enable counter */
	TIM_Cmd(TIM2, ENABLE);
}

/*
	Delay by the provided number of micro seconds.
	Limitation: "us" * System-Freq in MHz must now overflow in 32 bit.
	Values between 0 and 1.000.000 (1 second) are ok.
*/
void delay_micro_seconds(uint32_t us)
{
	TIM_SetCounter(TIM2, 0);
	/* use 16 bit count wrap around */
	while((uint16_t)(TIM2->CNT) <= us);
}

void i2c_command(uint8_t data)
{
	control = 0;
	i2c_out(data);
}

void I2C_stop()
{
	// Send I2C1 STOP Condition
	I2C_GenerateSTOP(I2C1, ENABLE);
}

void I2C_start(uint8_t address, uint8_t direction)
{
	// wait until I2C1 is not busy anymore
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

	// Send I2C1 START condition
	I2C_GenerateSTART(I2C1, ENABLE);

	// wait for I2C1 EV5 --> Slave has acknowledged start condition
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

	// Send slave Address for write
	I2C_Send7bitAddress(I2C1, address, direction);

	/* wait for I2C1 EV6, check if
	 * either Slave has acknowledged Master transmitter or
	 * Master receiver mode, depending on the transmission
	 * direction
	 */

	if(direction == I2C_Direction_Transmitter)
	{
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	}
	else if(direction == I2C_Direction_Receiver)
	{
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	}
}

void i2c_init(uint32_t ns)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef I2C_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_WriteBit(GPIOB, GPIO_Pin_5, (BitAction)RESET);

	I2C_DeInit(I2C1);

	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 400000;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_OwnAddress1 = 0;
	I2C_Init(I2C1, &I2C_InitStructure);

	I2C_Cmd(I2C1, ENABLE);

	delay_init();
}

void i2c_out(uint8_t data)
{
	I2C_start(SSD1306_I2C_ADDRESS, I2C_Direction_Transmitter);
	//Wire.write(control);
	I2C_SendData(I2C1, control);
	// wait for I2C1 EV8_2 --> byte has been transmitted
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_SendData(I2C1, data);
	// wait for I2C1 EV8_2 --> byte has been transmitted
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	I2C_stop();
}

/*========================================================================*/
/*
	The following delay procedures must be implemented for u8glib

	void u8g_Delay(uint16_t val)		Delay by "val" milliseconds
	void u8g_MicroDelay(void)		Delay be one microsecond
	void u8g_10MicroDelay(void)	Delay by 10 microseconds
*/

void u8g_Delay(uint16_t val)
{
	delay_micro_seconds(1000UL*(uint32_t)val);
}

void u8g_MicroDelay(void)
{
	delay_micro_seconds(1);
}

void u8g_10MicroDelay(void)
{
	delay_micro_seconds(10);
}

/*========================================================================*/
/* u8glib com procedure */
uint8_t u8g_com_hw_i2c_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{
	switch(msg)
	{
		case U8G_COM_MSG_STOP:
			break;
		case U8G_COM_MSG_INIT:
			i2c_init(0);
			u8g_MicroDelay();      
			break;
		case U8G_COM_MSG_ADDRESS:                     /* define cmd (arg_val = 0) or data mode (arg_val = 1) */
			if (arg_val == 0)
			{
				control = 0;
			}
			else
			{
				control = 0x40;
			}
			u8g_10MicroDelay();
			break;
		case U8G_COM_MSG_RESET:
			GPIO_WriteBit(GPIOB, GPIO_Pin_9, (BitAction)arg_val);
			u8g_10MicroDelay();
			break;
		case U8G_COM_MSG_WRITE_BYTE:
			i2c_out(arg_val);
			u8g_MicroDelay();
			break;
		case U8G_COM_MSG_WRITE_SEQ:
		case U8G_COM_MSG_WRITE_SEQ_P:
		{
			register uint8_t *ptr = arg_ptr;
			I2C_start(SSD1306_I2C_ADDRESS, I2C_Direction_Transmitter);
			I2C_SendData(I2C1, control);
			while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
			while(arg_val > 0)
			{
				I2C_SendData(I2C1, *ptr++);
				arg_val--;
				while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
			}
			I2C_stop();
			u8g_MicroDelay();
		}
		break;
	}
	return 1;
}
