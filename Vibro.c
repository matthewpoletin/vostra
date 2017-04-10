#include "Vibro.h"

void VibroInit(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;				// Включение тактирования порта B
	// Вибромотор 1
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;				// Включение тактирования регистра алтернативных функций
	AFIO->MAPR &= ~AFIO_MAPR_SWJ_CFG;				// Сброс настроек SWD
	AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE;	// Установка режима SWD (0b010/0x02)
	GPIOB->CRL &= ~(GPIO_CRL_CNF3 | GPIO_CRL_MODE3);// Сброс настроек
	GPIOB->CRL |= GPIO_CRL_MODE3_1;					// Push-Pull (0b00), частота 2 МГц (0b10)
	// Вибромотор 2
	GPIOB->CRH &= ~(GPIO_CRH_CNF9 | GPIO_CRH_MODE9);// Сброс настроек
	GPIOB->CRH |= GPIO_CRH_MODE9_1;					// Push-Pull (0b00), частота 2 МГц (0b10)
}
