#ifndef VIBRO
#define VIBRO

#include "stm32f10x.h"		// микроконтроллер stm32f100rb

#include "stdbool.h"		// библиотека булевых значений

static bool bEnableVibro1 = false;	// начальный статус работы вибромотора 1
static bool bEnableVibro2 = false;	// начальный статус работы вибромотора 2

void Vibro1Init(void);		// конфигурирование первого вибромотора	
void Vibro2Init(void);		// конфигурирование Вибромоторов

void EnableVibro1(void);	// разрешение работы вибромотора 1
void DisableVibro1(void);	// запрещение работы вибромотора 1
void EnableVibro2(void);	// разрешение работы вибромотора 2
void DisableVibro2(void);	// запрещение работы вибромотора 2

void Vibro1On(void);		// включение вибромотора 1
void Vibro1Off(void);		// выключение вибромотора 1
void Vibro1Toggle(void);	// смена состояния вибромотора 1
void Vibro2On(void);		// включение вибромотора 2
void Vibro2Off(void);		// выключение вибромотора 2
void Vibro2Toggle(void);	// смена состояния вибромотора 2

#endif	// VIBRO
