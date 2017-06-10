#include "stm32f10x.h"		// микроконтроллер stm32f100rb

#include "stdbool.h"		// библиотека булевых значений
#include "string.h"			// библиотека работы со строками

#include "Vibro.h"			// работа с вибромоторами
#include "Display.h"		// работа с дисплеем
#include "Bluetooth.h"		// работа с bluetooth-модулем
#include "Accel.h"			// работа с акселерометром
#include "Tools.h"			// дополнительные инструменты
#include "Button.h"			// работа с кнопкой

//void TIM_Configuration(void);	// настройка таймера

//unsigned int size = 0;			// количество поступающих поворотов
//bool directions[256];			// список поворотов
//unsigned int position = 0;		// номер текущего поворота
//bool checked = false;			// флаг совершения поворота

//unsigned int range = 1200;		// диапазон значений поворота

//char btData[256];

bool reset = false;

int main(void)
{
//	RCC_HSICmd(ENABLE);
//	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
	
//	Vibro1Init();
//	Vibro2Init();
//	Vibro1Off();
//	Vibro2Off();	
	DisplayInit();
	BluetoothInit();
	AccelInit();
//	TIM_Configuration();
	ButtonInit();
	
	char separatorString[] = "--------------------\n";
	UARTUpdateBuffer(separatorString);
	char welcomeString[] = "Vostra Version 1.7";
	UARTUpdateBuffer(welcomeString);
	
	DisplayLogo();
	delay(5000000);
	
	while(true)
	{
		if(reset)
		{
			SCB->AIRCR = 0x05fa0004;    // System RESET!!
			reset = false;         
		}
		
		BluetoothUpdate();
		DisplayDraw();
//		if(bEnableVibro)
//		{
//			if(GetX() > 4094/2) Vibro1On();
//			else Vibro1Off();
			
//		if(GetY() > 4094/2) Vibro2On();
//		else Vibro2Off();
//		}
//		else
//		{
//			Vibro1Off();
//			Vibro2Off();
//		}
	}
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
