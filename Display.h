#ifndef DISPLAY
#define DISPLAY

#include "stm32f10x.h"		// микроконтроллер stm32f100rb

#include "stdbool.h"		// библиотека булевых значений

#include <u8glib/u8g.h>		// графическая библиотека
#include "u8g_arm.h"		// обвязка под arm графической библиотеки

static u8g_t u8g;

static bool bEnableDisplay;

void DisplayInit(void);		// Конфигурирование дисплея

void DisplayLogo(void);		// Отрисовка картиники инициализации
void DisplayDraw(void);		// Отрисовка изображения

#endif	// DISPLAY
