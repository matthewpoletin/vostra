#ifndef BLUETOOTH
#define BLUETOOTH

#include "stm32f10x.h"		// Микроконтроллер

#include "string.h"
#include "stdbool.h"

#define RX_BUFF_SIZE 256
#define TX_BUFF_SIZE 256

extern char RxBuffer[RX_BUFF_SIZE];	// Буфер приёма USART
extern char TxBuffer[TX_BUFF_SIZE];	// Буфер передачи USART
extern bool bReceivedData;			// Маркер статуса приёма данных
extern bool bSendData;				// Маркер необходимости отправки данных

void BluetoothInit(void);			// Конфигурирование Bluetooth
void UARTUpdateBuffer(char* data);	// Запись данных в буфер передачи
void UARTSendBuffer(void);			// Отправка данных по UART

#endif	// BLUETOOTH
