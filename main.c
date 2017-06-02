#include "stm32f10x.h"
#include "stdbool.h"
#include "string.h"

#include "stm32f10x.h"                  // Device header
#include <u8glib/u8g.h>
#include "u8g_arm.h"

#include "Vibro.h"
#include "Display.h"
#include "Bluetooth.h"
#include "Accel.h"
#include "Tools.h"

void TIM_Configuration(void);	// настройка таймера
void ButtonConfiguration(void);	// настройка кнопки

unsigned int size = 0;			// количество поступающих поворотов
bool directions[256];			// список поворотов
unsigned int position = 0;		// номер текущего поворота
bool checked = false;			// флаг совершения поворота

unsigned int range = 1200;		// диапазон значений поворота

bool bEnableVibro = false;

u8g_t u8g;

unsigned int hours = 10;
unsigned int minutes = 41;
char time[10];

bool reset = false;

unsigned int pixelPosition = 0;

const uint8_t rook_bitmap[] U8G_PROGMEM = {
	0x00,			// 00000000 
	0x55,			// 01010101
	0x7f,			// 01111111
	0x3e,			// 00111110
	0x3e,			// 00111110 
	0x3e,			// 00111110
	0x3e,			// 00111110 
	0x7f			// 01111111
};

void draw(void)
{
	u8g_FirstPage(&u8g);  
	do
	{
		minutes++;
		sprintf(time, "%d:%d", hours, minutes);
		u8g_SetFont(&u8g, u8g_font_freedoomr10r);
		u8g_DrawStr(&u8g, 40, 20, time);
		if(minutes == 60) { minutes = 0; hours++; }
		if(hours == 24) { hours = 0; }
		
		u8g_DrawBitmapP(&u8g, 0, 0, 1, 8, rook_bitmap);
			
//			if(2 <= hours || hours <= 4) u8g_SleepOff(&u8g);
//			else u8g_SleepOn(&u8g);
		
		u8g_DrawPixel(&u8g, pixelPosition, 25);
		pixelPosition++;
		
//		u8g_DrawBox(&u8g, 0, 40, 127, 64);
//			u8g_DrawBitmapP(&u8g,0,10,13,19,(const uint8_t *)&img);
//			u8g_DrawFrame(&u8g, 0, 29, 127, 15);  
//			u8g.font = u8g_font_courR08;//u8g_font_profont22;  
//			u8g_DrawStr(&u8g, 10, 40, "Research Engeneer");
	} while ( u8g_NextPage(&u8g) ); 
	/* refresh screen after some delay */
	//delay_micro_seconds(10000);
}

int main(void)
{
	char welcome_str[] = "Vostra Version 1.5\r\n";
	UARTUpdateBuffer(welcome_str);
	
	VibroInit();
//	DisplayInit();
	BluetoothInit();
	AccelInit();
//	TIM_Configuration();
	ButtonConfiguration();
	// Either use normal display driver (slower, small memory requirement:
	u8g_InitComFn(&u8g, &u8g_dev_ssd1306_128x64_i2c, u8g_com_hw_i2c_fn);
	// or the 2x driver (faster, but more memory requirement.)
	//u8g_InitComFn(&u8g, &u8g_dev_ssd1306_128x64_2x_i2c, u8g_com_hw_i2c_fn);  
	// See the bottom of file: u8g_dev_ssd1306_128x64.c
	
		
	while(true)
	{
		if(reset)
		{
			u8g_InitComFn(&u8g, &u8g_dev_ssd1306_128x64_i2c, u8g_com_hw_i2c_fn);
			u8g_Re
			reset = false;         
		}
		
		draw();
		if(bEnableVibro)
		{
//			if(GetX() > 4094/2) Vibro1On();
//			else Vibro1Off();
			
			if(GetY() > 4094/2)Vibro2On();
			else Vibro2Off();
		}
		else
		{
//			Vibro1Off();
			Vibro2Off();
		}
		
		if(bReceivedData)	
		{
			Vibro1Toggle();
			Vibro2Toggle();
						
			memset(RxBuffer, 0, sizeof(RxBuffer));	// Очистка буфера
			bReceivedData = false;					// Сброс статуса получения команды
		}
		if(bSendData)
		{
			UARTSendBuffer();
		}
	}
}

void ButtonConfiguration(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;	// Включение тактирования порта C
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;	// Включение тактирования альтернативных функций
	
	GPIOA->CRH &= ~(GPIO_CRH_CNF13 | GPIO_CRH_MODE13);	// Сброс настроек
	GPIOA->CRH |= GPIO_CRH_CNF13_1;				// Включение режима с подтяжкой
	
	AFIO->EXTICR[3] |= AFIO_EXTICR4_EXTI13_PC;	// Настройка альтернативной функции на PC13
	
	EXTI->RTSR |= EXTI_RTSR_TR13; 				// Прерывание по восходящему фронту
	EXTI->IMR |= EXTI_IMR_MR13;					// Разрешение прерывания по линии 13
	
	NVIC_EnableIRQ(EXTI15_10_IRQn); 			// Включение прерывания 
	NVIC_SetPriority(EXTI15_10_IRQn, 0);		// Установка приоритета прерываний
}

// Обработчик прерываний нажатия кнопки
void EXTI15_10_IRQHandler()
{	
	delay(10000);					// Задержка для фильтрации ложных срабатываний	
	
	if ((GPIOC->IDR & GPIO_IDR_IDR13) > 0)
	{
		reset = true;
		
		
		bEnableVibro = !bEnableVibro;
		
		char str[20];
		sprintf(str, "%d - %d - %d\r\n", GetX(), GetY(), GetZ());
		UARTUpdateBuffer(str);
	}
	
	EXTI->PR |= EXTI_PR_PR0;		// Сброс флага
}

//void TIM_Configuration(void)
//{
//	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;	// Включение тактирования TIM3
//	
//	TIM3->PSC = 24000 - 1;		// Set prescaler to 24 000 (PSC + 1)
//	TIM3->ARR = 4000;			// Auto reload value 1000
//	TIM3->DIER = TIM_DIER_UIE;	// Включение прерываний 

//	NVIC_EnableIRQ(TIM3_IRQn);	// Включение прерываний
//}

//// Обработчик прерывания окончания работы таймера
//void TIM3_IRQHandler(void)
//{
//	if(TIM3->SR & TIM_SR_UIF)
//	{
//		TIM3->SR &= ~TIM_SR_UIF;						// сброс флага
//		Vibro1Off();
//		Vibro2Off();
//		UARTUpdateBuffer("Stop Vibration\r\n");			// Заполнение буфера передачи
//		TIM3->CR1 &= ~TIM_CR1_CEN;						// Выключение таймера
//	}
//}
