#include "Vibro.h"

void VibroInit(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;				// Включение тактирования порта B
	// Вибромотор 1
	GPIOB->CRH &= ~(GPIO_CRH_CNF8 | GPIO_CRH_MODE8);// Сброс настроек
	GPIOB->CRH |= GPIO_CRH_MODE8_1;					// Push-Pull (00), частота 2 МГц (10)
	// Вибромотор 2
	GPIOB->CRH &= ~(GPIO_CRH_CNF9 | GPIO_CRH_MODE9);// Сброс настроек
	GPIOB->CRH |= GPIO_CRH_MODE9_1;					// Push-Pull (00), частота 2 МГц (10)
}
