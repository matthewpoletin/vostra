#include "Interface.h"

#include <stdbool.h>

unsigned int hours = 4;
unsigned int minutes = 20;

int unreadMessages = 0;

float temperatureCelc = +15.3f;
bool bUseFahrenheit = false;
float temperatureFahr;

enum weatherType weather = wtSunny;