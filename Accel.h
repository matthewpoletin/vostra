#ifndef ACCEL
#define ACCEL

#include "stm32f10x.h"		// микроконтроллер stm32f100

void AccelInit(void);		// Инициализация акселерометра

int GetX(void);				// Получение значения по оси X
int GetY(void);				// Получение значения по оси Y
int GetZ(void);				// Получение значения по оси Z

#endif	// ACCEL
