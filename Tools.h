#ifndef TOOLS
#define TOOLS

#include "stm32f10x.h"		// Микроконтроллер

// Задержка на заданное количество счета
void delay(uint32_t delayTime)
{
	for(; 0 < delayTime; delayTime--) {}
}

#endif	// TOOLS
