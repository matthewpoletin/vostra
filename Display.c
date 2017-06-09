#include "Display.h"

#include "stdbool.h"		// библиотека булевых значений

#include "Bitmaps.h"		// доступ к изображениям
#include "Interface.h"		// работа с интерфейсом
// TODO: перенести интеграцию с bt в файлы UI
#include "Bluetooth.h"		// работа с bluetooth-модулем

// TODO: перенести в соостветсвующие файлы
// Время
//unsigned int hours = 4;
//unsigned int minutes = 20;
//unsigned int seconds = 0;
//char timeString[10];

//char data[256];

enum weatherType
{
	wtDefault = 1,
	wtSunny,
	wtRainy,
	wtCloudy
};

enum weatherType weather = wtRainy;

// Температура
float temperatureCelc = +15.3f;
bool bUseFahrenheit = false;
//T(°F) = T(°C) × 9/5 + 32
float temperatureFahr;
char temperatureString[10];

char unreadMessagesString[2];
unsigned int unreadMessages = 1;

void DisplayInit(void)
{
	// Either use normal display driver (slower, small memory requirement:
	u8g_InitComFn(&u8g, &u8g_dev_ssd1306_128x64_i2c, u8g_com_hw_i2c_fn);
	// or the 2x driver (faster, but more memory requirement.)
	//u8g_InitComFn(&u8g, &u8g_dev_ssd1306_128x64_2x_i2c, u8g_com_hw_i2c_fn);  
	// See the bottom of file: u8g_dev_ssd1306_128x64.c
	
	bEnableDisplay = true;
}

void DisplayLogo(void)
{
	if(bEnableDisplay)
	{
		u8g_FirstPage(&u8g);  
		do
		{
			u8g_DrawXBM(&u8g, 0, 0, cityBitmapWidth, cityBitmapHeight, cityBitmap);
			u8g_SetFont(&u8g, u8g_font_freedoomr10r);
			u8g_DrawStr(&u8g, 79, 20, "VOSTRA");
			
//			u8g_DrawXBM(&u8g, 0, 0, 38, 21, bearBitmap);
//			u8g_SetFont(&u8g, u8g_font_freedoomr10r);
//			u8g_DrawStr(&u8g, 40, 20, "VOSTRA");
		} while ( u8g_NextPage(&u8g) );
	}
}

void DisplayDraw(void)
{
//	strcpy(data, RxBuffer);
//	
	u8g_FirstPage(&u8g);  
	do
	{
		// Температура
		if(bUseFahrenheit)
		{
			temperatureFahr = temperatureCelc * (9/2) + 32;
			sprintf(temperatureString, "%+2.1f°F", temperatureFahr);
		}
		else
		{
			sprintf(temperatureString, "%+02.1f C", temperatureCelc);
		}
		u8g_SetFont(&u8g, u8g_font_freedoomr10r);
		u8g_DrawStr(&u8g, 75, 60, temperatureString);
		u8g_DrawCircle(&u8g, 75+40, 60-13, 2, U8G_DRAW_ALL);
		switch(weather)
		{
			case(wtSunny):
				u8g_DrawXBM(&u8g, 40, 35, sunnyBitmapWidth, sunnyBitmapHeight, sunnyBitmap);
				break;
			case(wtCloudy):
				u8g_DrawXBM(&u8g, 40, 35, cloudyBitmapWidth, cloudyBitmapHeight, cloudyBitmap);
				break;
			case(wtRainy):
				u8g_DrawXBM(&u8g, 40, 35, rainyBitmapWidth, rainyBitmapHeight, rainyBitmap);
				break;
			default:
				break;
		}
		
		// Время
		seconds++;
		sprintf(timeString, "%02d:%02d", hours, minutes);
		u8g_SetFont(&u8g, u8g_font_freedoomr25n);
		u8g_DrawStr(&u8g, 40, 35, timeString);
		if(seconds == 60) { seconds = 0; minutes++; }
		if(minutes == 60) { minutes = 0; hours++; }
		if(hours == 24) { hours = 0; }
		
		// Сообщения
		u8g_DrawXBM(&u8g, 0, 20, envelopeBitmapWidth, envelopeBitmapHeight, envelopeBitmap);
		u8g_SetColorIndex(&u8g, 0);		// Обратный цвет
		if(unreadMessages > 0)
		{
			u8g_DrawDisc(&u8g, 0+envelopeBitmapWidth, 20+envelopeBitmapHeight, 6, U8G_DRAW_ALL);
			u8g_SetColorIndex(&u8g, 1);		// Прямой цвет
			u8g_DrawCircle(&u8g, 0+envelopeBitmapWidth, 20+envelopeBitmapHeight, 6, U8G_DRAW_ALL);
			u8g_SetFont(&u8g, u8g_font_helvR08);
			
			if(unreadMessages > 9) sprintf(unreadMessagesString, "~");
			else sprintf(unreadMessagesString, "%1d", unreadMessages);
			u8g_DrawStr(&u8g, 0+envelopeBitmapWidth-2, 20+envelopeBitmapHeight+5, unreadMessagesString);
		}
		
		// Проверочно
//		u8g_SetFont(&u8g, u8g_font_freedoomr10r);
//		u8g_DrawStr(&u8g, 40, 60, data);
//		u8g_DrawBitmapP(&u8g, 0, 0, 1, 8, rook_bitmap);
		
//		if(2 <= hours || hours <= 4) u8g_SleepOff(&u8g);
//		else u8g_SleepOn(&u8g);
		
//		u8g_DrawPixel(&u8g, pixelPosition, 25);
//		pixelPosition++;
		
//		u8g_DrawBox(&u8g, 0, 40, 127, 64);
//		u8g_DrawFrame(&u8g, 0, 29, 127, 15);
	} while ( u8g_NextPage(&u8g) ); 
	/* refresh screen after some delay */
	//delay_micro_seconds(10000);
}
