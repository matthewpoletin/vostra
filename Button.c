#include "Button.h"

#include "string.h"			// 
#include "stm32f10x.h"		// Device header
#include "Bluetooth.h"		// 
#include "Accel.h"			// 
#include "Tools.h"			// 

void ButtonInit(void)
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
		//reset = true;
			
		//bEnableVibro1 = !bEnableVibro1;
		
//		char str[20];
//		sprintf(str, "%d - %d - %d\r\n", GetX(), GetY(), GetZ());
//		UARTUpdateBuffer(str);
	}
	
	EXTI->PR |= EXTI_PR_PR0;		// Сброс флага
}
