#include "stm32f10x.h"
#include "stdbool.h"
#include "string.h"

#include "Vibro.h"
#include "Display.h"
#include "Bluetooth.h"
#include "Accel.h"
#include "Tools.h"

void TIM_Configuration(void);		// Настройка таймера
void Button_Configuration(void);	// Настройка кнопки

unsigned int size = 0;			// количество поступающих поворотов
bool directions[256];			// список поворотов
unsigned int position = 0;		// номер текущего поворота
bool checked = false;			// флаг совершения поворота

unsigned int range = 1200;		// диапазон значений поворота

int main(void)
{
	char welcome_str[] = "Vostra Version 1.4\r\n";
	UARTUpdateBuffer(welcome_str);
	
	VibroInit();
	//DisplayInit();
	LEDInit();
	BluetoothInit();
	AccelInit();
	TIM_Configuration();
	Button_Configuration();
	
	while(true)
	{
		if(position < size)
		{
			if(directions[position] == true)
			{
				Vibro1On();
				Vibro2Off();
			}
			else
			{
				Vibro2On();
				Vibro1Off();
			}
			
			if(checked == false)
			{
				if(0 <= GetX() && GetX() <= 0 + range)
				{
					if(directions[position] == true)
					{
						checked = true;
						UARTUpdateBuffer("Turning right detected\r\n");
					}
				}
				else if(4095 - range < GetX() && GetX() <= 4095)
				{
					if(directions[position] == false)
					{
						checked = true;
						UARTUpdateBuffer("Turning left detected\r\n");
					}
				}
			}
			else if(0 + range < GetX() && GetX() <= 4095 - range)
			{
				if(checked)
				{
					UARTUpdateBuffer("Turning ended\r\n");
					Vibro1Off();
					Vibro2Off();
					delay(0xFFFFF);
					position++;
					checked = false;
					if(position == size)
					{
						Vibro1Off();
						Vibro2Off();
						UARTUpdateBuffer("Movment finished\r\n");
					}
				}
			}
		}
		
		if(bReceivedData)	
		{
			size = strlen(RxBuffer);
			for(unsigned int counter = 0; counter < size; counter++)
			{
				if(RxBuffer[counter] == 'r')
				{
					directions[counter] = true;
				}
				else if(RxBuffer[counter] == 'l')
				{
					directions[counter] = false;
				}
			}
						
			memset(RxBuffer, 0, sizeof(RxBuffer));	// Очистка буфера
			bReceivedData = false;					// Сброс статуса получения команды
		}
		if(bSendData)
		{
			UARTSendBuffer();
		}
	}
}

void Button_Configuration(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;	// Включение тактирования порта A
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;	// Включение тактирования альтернативных функций
	
	GPIOA->CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_MODE0);	// Сброс настроек
	GPIOA->CRL |= GPIO_CRL_CNF0_1;				// Включение режима с подтяжкой
	
	AFIO->EXTICR[0] |= AFIO_EXTICR1_EXTI0_PA;	// Настройка альтернативной функции на PA0
	
	EXTI->RTSR |= EXTI_RTSR_TR0; 				// Прерывание по восходящему фронту
	EXTI->IMR |= EXTI_IMR_MR0;					// Разрешение прерывания по линии 0
	
	NVIC_EnableIRQ(EXTI0_IRQn); 				// Включение прерывания 
	NVIC_SetPriority(EXTI0_IRQn, 0);			// Установка приоритета прерываний
}

// Обработчик прерываний нажатия кнопки
void EXTI0_IRQHandler()
{	
	delay(10000);					// Задержка для фильтрации ложных срабатываний	
	
	if ((GPIOA->IDR & GPIO_IDR_IDR0) > 0)
	{
		LEDToggle();
		
			int tmp = GetX();
			char str[15];
			sprintf(str, "%d\r\n", tmp);
			UARTUpdateBuffer(str);
	}
	
	EXTI->PR |= EXTI_PR_PR0;		// Сброс флага
}

void TIM_Configuration(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;	// Включение тактирования TIM3
	
	TIM3->PSC = 24000 - 1;		// Set prescaler to 24 000 (PSC + 1)
	TIM3->ARR = 4000;			// Auto reload value 1000
	TIM3->DIER = TIM_DIER_UIE;	// Включение прерываний 

	NVIC_EnableIRQ(TIM3_IRQn);	// Включение прерываний
}

// Обработчик прерывания окончания работы таймера
void TIM3_IRQHandler(void)
{
	if(TIM3->SR & TIM_SR_UIF)
	{
		TIM3->SR &= ~TIM_SR_UIF;						// сброс флага
		Vibro1Off();
		Vibro2Off();
		UARTUpdateBuffer("Stop Vibration\r\n");			// Заполнение буфера передачи
		TIM3->CR1 &= ~TIM_CR1_CEN;						// Выключение таймера
	}
}
