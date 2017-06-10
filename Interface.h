#ifndef INTERFACE
#define INTERFACE

#include <stdbool.h>		// 

// Время
extern unsigned int hours;
extern unsigned int minutes;
static unsigned int seconds = 0;
static char timeString[10];

// Сообщения
static char unreadMessagesString[2];
extern int unreadMessages;

// Температура
enum weatherType
{
	wtDefault = 1,
	wtSunny,
	wtRainy,
	wtCloudy
};
extern enum weatherType weather;
extern float temperatureCelc;
extern bool bUseFahrenheit;
//T(°F) = T(°C) × 9/5 + 32
extern float temperatureFahr;
static char temperatureString[10];

void InterfaceInit(void);		// Инициализация интерфейса

#endif	// INTERFACE
