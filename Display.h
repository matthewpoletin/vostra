#ifndef DISPLAY
#define DISPLAY

#include "stm32f10x.h"		// Микроконтроллер

#include <u8glib/u8g.h>
#include "u8g_arm.h"

static u8g_t u8g;

void DisplayInit(void);		// Конфигурирование дисплея

void DisplayLogo(void);
void DisplayDraw(void);		// Отрисовка изображения

#endif	// DISPLAY
