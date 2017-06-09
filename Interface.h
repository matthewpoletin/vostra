#ifndef INTERFACE
#define INTERFACE

extern unsigned int hours;
extern unsigned int minutes;
static unsigned int seconds = 0;
static char timeString[10];

void InterfaceInit(void);		// Инициализация интерфейса

#endif	// INTERFACE
