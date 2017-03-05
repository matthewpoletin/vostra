#ifndef ACCEL
#define ACCEL

#include "stm32f10x.h"		// Микроконтроллер

void AccelInit(void);	// Инициализация акселерометра

int GetX(void);
int GetY(void);
int GetZ(void);

#endif	// ACCEL
