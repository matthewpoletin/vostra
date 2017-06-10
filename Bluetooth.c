#include "Bluetooth.h"

#include "stdio.h"			// обработка строк
#include <stdlib.h>			// преобразование форматов

#include "Interface.h"		// работа с интерфейсом

char RxBuffer[RX_BUFF_SIZE];
char TxBuffer[TX_BUFF_SIZE];
bool bReceivedData = false;		// Есть необработанные данные буфера
bool bSendData = false;			// Нужно отправить данные буфера

extern unsigned int hours;
extern int unreadMessages;

void BluetoothInit(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;		// Включение тактирования порта А
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;		// Включение тактирования альтернативных функций на шине
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;	// Включение тактирования USART1
	
	//Настраиваем PA9 на выход
	GPIOA->CRH &= ~(GPIO_CRH_CNF9 | GPIO_CRH_MODE9);	// Сброс настроек
	GPIOA->CRH |= (GPIO_CRH_CNF9_1 | GPIO_CRH_MODE9_1);	// Push-Pull (00), частота 2 МГц (10)
	//Оставляем PA10 настроен по умолчанию
	
	//USART1->BRR = 0x9C4;						// Настраиваем частоту тактирования
	USART1->BRR = 0x34D;						// Настраиваем частоту тактирования
	
	USART1->CR1 |= USART_CR1_UE;				// Включение USART
	USART1->CR1 |= USART_CR1_TE | USART_CR1_RE;	// Включение приемник и передатчик
	USART1->CR1 |= USART_CR1_RXNEIE;			// Включение прерывания по поступлению данных
	
	NVIC_EnableIRQ(USART1_IRQn);				// Подключение функции обработчика прерываний
}

void UARTUpdateBuffer(char* data)
{
	strcat(TxBuffer, data);
	//strcpy(&TxBuffer[0], data);
	bSendData = true;
}

void UARTSendBuffer()
{
	strcat(TxBuffer, "\r\n");
	uint8_t counter;		// Счетчик
	// Цикл до передачи всех симвлолов
	for (counter = 0; counter < strlen(TxBuffer); counter++)
	{
		while ((USART1->SR & USART_SR_TC) == 0) {};	// TC (transmit complete)
		USART1->DR = TxBuffer[counter];				// Запись байта в data register
	}
	
	memset(&TxBuffer[0], 0, sizeof(TxBuffer));
	bSendData = false;
}

// Обработчик прерывания приёма данных
void USART1_IRQHandler(void)
{
	char tmp;								// Временная переменная одного символа
	if ((USART1->SR & USART_SR_RXNE) != 0)	// Прерывание по приёму данных
	{
		tmp = USART1->DR;					// Запись из регистр данных
		if (tmp != 0x0D)					// Если это не символ конца строки
		{
			RxBuffer[strlen(RxBuffer)] = tmp;
		}
		else bReceivedData = true;			// Сообщение о приёме
	}
}

void BluetoothUpdate(void)
{
	if(bReceivedData)	
	{
		InputProcessing();
				
		memset(RxBuffer, 0, sizeof(RxBuffer));	// Очистка буфера приёма
		bReceivedData = false;					// Установка статуса обработки данных
	}
	if(bSendData)
	{
		UARTSendBuffer();
	}
}

void InputProcessing(void)
{
	char command[10];
	char parameter[10];
	//unsigned int parameter; // TODO: поменять на char
	
//	sscanf(RxBuffer, "%[^:]: %s;\n", command, parameter);
	
	sscanf(RxBuffer, "%[^:]: %[^;];\n", command, parameter);
	
	if(strcmp(command, "hours") == 0)
	{
		//hours = parameter;
		UARTUpdateBuffer("Часы выставлены");
	}
	else if(strcmp(command, "minutes") == 0)
	{
		//minutes = parameter;
		seconds = 0;
		UARTUpdateBuffer("Минуты выставлены");
	}
	else if(strcmp(command, "messages") == 0)
	{
		//unreadMessages += parameter;
		int temp = atoi(parameter);
		unreadMessages += temp;
		if (unreadMessages < 0) unreadMessages = 0;
		UARTUpdateBuffer("Добавлены сообщения");
	}
	else if(strcmp(command, "temperature") == 0)
	{
		float temp = atof(parameter);
		if(-100 < temp && temp < 100)
		{			
			temperatureCelc = temp;
			//temperatureCelc = parameter;
			UARTUpdateBuffer("Температура выставлена");
		}
		else
		{
			UARTUpdateBuffer("Ошибка выставления температуры");
		}
	}
	else if(strcmp(command, "weather") == 0)
	{
		if(strcmp(parameter, "sunny") == 0)
		{
			weather = wtSunny;
			UARTUpdateBuffer("Солнечно");
		}
		else if(strcmp(parameter, "cloudy") == 0)
		{
			weather = wtCloudy;
			UARTUpdateBuffer("Облачно");
		}
		else if(strcmp(parameter, "rainy") == 0)
		{
			weather = wtRainy;
			UARTUpdateBuffer("Дождливо");
		}
		UARTUpdateBuffer("Выставлена погода");
	}
	else
	{
		// TODO: добавить вывод сообщения об ошибке
		UARTUpdateBuffer("Неверная команда: \"");
		UARTUpdateBuffer(RxBuffer);
		UARTUpdateBuffer("\"");
	}
}
