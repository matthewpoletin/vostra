#include "Bluetooth.h"

char RxBuffer[RX_BUFF_SIZE];
char TxBuffer[TX_BUFF_SIZE];
bool bReceivedData = false;
bool bSendData = false;

void BluetoothInit(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;		// Включение тактирования порта А
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;		// Включение тактирования альтернативных функций на шине
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;	// Включение тактирования USART1
	
	//Настраиваем PA9 на выход
	GPIOA->CRH &= ~(GPIO_CRH_CNF9 | GPIO_CRH_MODE9);	// Сброс настроек
	GPIOA->CRH |= (GPIO_CRH_CNF9_1 | GPIO_CRH_MODE9_1);	// Push-Pull (00), частота 2 МГц (10)
	//Оставляем PA10 настроен по умолчанию
	
	USART1->BRR = 0x9C4;						// Настраиваем частоту тактирования
	USART1->CR1 |= USART_CR1_UE;				// Включение USART
	USART1->CR1 |= USART_CR1_TE | USART_CR1_RE;	// Включение приемник и передатчик
	USART1->CR1 |= USART_CR1_RXNEIE;			// Включение прерывания по поступлению данных
	
	NVIC_EnableIRQ(USART1_IRQn);				// Подключение функции обработчика прерываний
}

void UARTUpdateBuffer(char* data)
{
	strcpy(&TxBuffer[0], data);
	bSendData = true;
}

void UARTSendBuffer()
{
	uint8_t counter = strlen(TxBuffer);		// Счетчик
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
		else bReceivedData = true;		// Сообщение о приёме
	}
}
