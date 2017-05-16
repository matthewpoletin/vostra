#include "Accel.h"

void AccelInit(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;		// включение тактирования порта C
	GPIOC->CRL &= ~(GPIO_CRL_MODE1 | GPIO_CRL_CNF1);	//  аналоговый вход PC1
	GPIOC->CRL &= ~(GPIO_CRL_MODE2 | GPIO_CRL_CNF2);	//  аналоговый вход PC2
	GPIOC->CRL &= ~(GPIO_CRL_MODE3 | GPIO_CRL_CNF3);	//  аналоговый вход PC3
	
	GPIOA->CRL &= ~GPIO_CRL_CNF1;			// настраиваем вывод для работы АЦП в режим аналогового входа
	RCC->CFGR |= RCC_CFGR_ADCPRE_DIV8;		// так как тактовая частота АЦП не должна превышать 14MHz
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;		// включение тактирования АЦП
	
	ADC1->CR2 |= ADC_CR2_CAL;				// запуск калибровки
	while (!(ADC1->CR2 & ADC_CR2_CAL)) {}	// ожидание окончания калибровки
	
	ADC1->CR2 |= ADC_CR2_JEXTSEL;	// выбрать источником запуска разряд  JSWSTART
	ADC1->CR2 |= ADC_CR2_JEXTTRIG;	// разр. внешний запуск инжектированной группы
	ADC1->CR2 |= ADC_CR2_CONT;		// режим непрерывного преобразования 
	ADC1->CR1 |= ADC_CR1_SCAN;		// режим сканирования (т.е. несколько каналов)
	ADC1->CR1 |= ADC_CR1_JAUTO;		// автомат. запуск инжектированной группы
	ADC1->JSQR |= (uint32_t)(3-1)<<20;	// длинна инжектированной области 3 преобразования (10)
	ADC1->JSQR |= (uint32_t)3<<(5*1);	// номер канала для первого преобразования
	ADC1->JSQR |= (uint32_t)4<<(5*2);	// номер канала для второго преобразования
	ADC1->JSQR |= (uint32_t)5<<(5*3);	// номер канала для третьего преобразования
	ADC1->CR2 |= ADC_CR2_ADON;		// включить АЦП
	ADC1->CR2 |= ADC_CR2_JSWSTART;	// запустить процес преобразования
}

int GetX(void)
{
	return ADC1->JDR1;
}

int GetY(void)
{
	return ADC1->JDR2;
}

int GetZ(void)
{
	return ADC1->JDR3;
}