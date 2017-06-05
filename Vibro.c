#include "Vibro.h"

void Vibro1Init(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;				// Включение тактирования порта B
	// Вибромотор 1
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;				// Включение тактирования регистра алтернативных функций
	AFIO->MAPR &= ~AFIO_MAPR_SWJ_CFG;				// Сброс настроек SWD
	AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE;	// Установка режима SWD (0b010/0x02)
	GPIOB->CRL &= ~(GPIO_CRL_CNF3 | GPIO_CRL_MODE3);// Сброс настроек
	GPIOB->CRL |= GPIO_CRL_MODE3_1;					// Push-Pull (0b00), частота 2 МГц (0b10)
}

void Vibro2Init(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;				// Включение тактирования порта B
	// Вибромотор 2
	GPIOB->CRH &= ~(GPIO_CRH_CNF9 | GPIO_CRH_MODE9);// Сброс настроек
	GPIOB->CRH |= GPIO_CRH_MODE9_1;					// Push-Pull (0b00), частота 2 МГц (0b10)
}

void EnableVibro1(void)
{
	bEnableVibro1 = true;
}

void DisableVibro1(void)
{
	bEnableVibro1 = false;
}

void EnableVibro2(void)
{
	bEnableVibro2 = true;
}

void DisableVibro2(void)
{
	bEnableVibro2 = false;
}

void Vibro1On(void)
{
	if(bEnableVibro1)	GPIOB->BSRR	|= GPIO_BSRR_BS3;
}

void Vibro1Off(void)
{
	GPIOB->BSRR	|= GPIO_BSRR_BR3;
}

void Vibro1Toggle(void)
{
	if(bEnableVibro1)	GPIOB->ODR	^= GPIO_ODR_ODR3;
}

void Vibro2On(void)
{
	if(bEnableVibro2)	GPIOB->BSRR	|= GPIO_BSRR_BS9;
}

void Vibro2Off(void)
{
	GPIOB->BSRR	|= GPIO_BSRR_BR9;
}

void Vibro2Toggle(void)
{
	if(bEnableVibro2)	GPIOB->ODR	^= GPIO_ODR_ODR9;
}
