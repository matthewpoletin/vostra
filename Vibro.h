#ifndef VIBRO
#define VIBRO

#include "stm32f10x.h"		// Микроконтроллер

void VibroInit(void);		// конфигурирование Вибромоторов

#define Vibro1On()		GPIOB->BSRR	|= GPIO_BSRR_BS8	// включение вибромотора 1
#define Vibro1Off()		GPIOB->BSRR	|= GPIO_BSRR_BR8	// выключение вибромотора 1
#define Vibro1Toggle()	GPIOB->ODR	^= GPIO_ODR_ODR8	// смена состояния вибромотора 1

#define Vibro2On()		GPIOB->BSRR	|= GPIO_BSRR_BS9	// включение вибромотора 2
#define Vibro2Off()		GPIOB->BSRR	|= GPIO_BSRR_BR9	// выключение вибромотора 2
#define Vibro2Toggle()	GPIOB->ODR	^= GPIO_ODR_ODR9	// смена состояния вибромотора 2

#endif	// VIBRO
