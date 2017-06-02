#ifndef _U8G_ARM_H
#define _U8G_ARM_H

#include <u8glib/u8g.h>
#include "stm32f10x_conf.h"

#define SYS_CLK 46000000    /* in this example we use SPEED_HIGH = 48 MHz */
#define DELAY_TIM_FREQUENCY 1000000 /* = 1MHZ -> timer runs in microseconds */
#define SSD1306_I2C_ADDRESS   0x78	// 011110+SA0+RW - 0x3C or 0x3D

#define SSD1306_SEGREMAP 0xA0
#define SSD1306_COMSCANINC 0xC0

static u8g_t u8g;

void delay_init(void);
//void delay_system_ticks(uint32_t sys_ticks);	
void delay_micro_seconds(uint32_t us);

void u8g_Delay(uint16_t val);
void u8g_MicroDelay(void);
void u8g_10MicroDelay(void);

void I2C_stop(void);
void I2C_start(uint8_t address, uint8_t direction);

void i2c_init(uint32_t ns) U8G_NOINLINE;
void i2c_out(uint8_t data);
void i2c_command(uint8_t data);

uint8_t u8g_com_hw_i2c_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);

#endif	// _U8G_ARM_H
