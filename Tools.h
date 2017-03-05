#ifndef TOOLS
#define TOOLS

#include "stm32f10x.h"		// Микроконтроллер

// Отладочный светодиод
void LEDInit(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;					// Включение тактирования порта C
	GPIOC->CRH &= ~(GPIO_CRH_MODE9 | GPIO_CRH_CNF9);	// Сброс изначальных настроек
	GPIOC->CRH |= GPIO_CRH_MODE9_1;						// Установка частоты тактирования 2 МГц
}

#define LEDOn() GPIOC->BSRR |= GPIO_BSRR_BS9	// включение отладочного светодиода
#define LEDOff() GPIOC->BSRR |= GPIO_BSRR_BR9	// выключение отладочного светодиода
#define LEDToggle() GPIOC->ODR ^= GPIO_ODR_ODR9	// смена состояния светодиода

// Задержка на заданное количество счета
void delay(uint32_t delayTime)
{
	for(; 0 < delayTime; delayTime--) {}
}

#endif	// TOOLS
