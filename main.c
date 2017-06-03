#include "stm32f10x.h"		// устройство

#include "stdbool.h"
#include "string.h"

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

bool bEnableVibro = true;

bool reset = false;

int main(void)
{
	char welcome_str[] = "Vostra Version 1.5\r\n";
	UARTUpdateBuffer(welcome_str);
	
	VibroInit();
	DisplayInit();
	BluetoothInit();
	AccelInit();
//	TIM_Configuration();
	ButtonConfiguration();
			
	while(true)
	{
		//Vibro1On();
		//Vibro2On();
		
		if(reset)
		{
			SCB->AIRCR = 0x05fa0004;    // System RESET!!
			//RCC->BDCR |= RCC_BDCR_BDRST;
			//RCC->BDCR &= ~RCC_BDCR_BDRST;
			//u8g_InitComFn(&u8g, &u8g_dev_ssd1306_128x64_i2c, u8g_com_hw_i2c_fn);
			//u8g_Re
			reset = false;         
		}
		
		DisplayDraw();
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
