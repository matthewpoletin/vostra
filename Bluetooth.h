#ifndef BLUETOOTH
#define BLUETOOTH

#include "stm32f10x.h"		// микроконтроллер stm32f100

#include "stdbool.h"		// библиотека булевых значений
#include "string.h"			// библиотека работы со строками

#define RX_BUFF_SIZE 256
#define TX_BUFF_SIZE 256

extern char RxBuffer[RX_BUFF_SIZE];	// Буфер приёма USART
extern char TxBuffer[TX_BUFF_SIZE];	// Буфер передачи USART
extern bool bReceivedData;			// Маркер статуса приёма данных
extern bool bSendData;				// Маркер необходимости отправки данных

void BluetoothInit(void);			// Конфигурирование Bluetooth
void UARTUpdateBuffer(char* data);	// Запись данных в буфер передачи
void UARTSendBuffer(void);			// Отправка данных по UART

void BluetoothUpdate(void);			// Обновление состояния

void InputProcessing(void);			// Обработка входящих команд

#endif	// BLUETOOTH
