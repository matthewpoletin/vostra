#include "stm32f10x.h"
#include "stdbool.h"
#include "string.h"

#include "Vibro.h"
#include "Display.h"
#include "Bluetooth.h"
#include "Accel.h"
#include "Tools.h"

void TIM_Configuration(void);		// Настройка таймера
void ButtonConfiguration(void);	// Настройка кнопки

unsigned int size = 0;			// количество поступающих поворотов
bool directions[256];			// список поворотов
unsigned int position = 0;		// номер текущего поворота
bool checked = false;			// флаг совершения поворота

unsigned int range = 1200;		// диапазон значений поворота

int main(void)
{
	char welcome_str[] = "Vostra Version 1.5\r\n";
	UARTUpdateBuffer(welcome_str);
	
	VibroInit();
//	DisplayInit();
//	LEDInit();
	BluetoothInit();
//	AccelInit();
//	TIM_Configuration();
	ButtonConfiguration();
	
	Vibro1On();
	Vibro2On();
	
	while(true)
	{
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
	
	
	EXTI->RTSR |= EXTI_RTSR_TR13; 				// Прерывание по восходящему фронту
	
}

// Обработчик прерываний нажатия кнопки
void EXTI15_10_IRQHandler()
{	
	delay(10000);					// Задержка для фильтрации ложных срабатываний	
	
	if ((GPIOC->IDR & GPIO_IDR_IDR13) > 0)
	{
		Vibro1Toggle();
		Vibro2Toggle();
		
//		int tmp = GetX();
//		char str[15];
//		sprintf(str, "%d\r\n", tmp);
//		UARTUpdateBuffer(str);
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
