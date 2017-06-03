#include "Display.h"

unsigned int hours = 4;
unsigned int minutes = 20;
char time[10];

unsigned int pixelPosition = 0;
const uint8_t rook_bitmap[] U8G_PROGMEM = {
	0x00,			// 00000000 
	0x55,			// 01010101
	0x7f,			// 01111111
	0x3e,			// 00111110
	0x3e,			// 00111110 
	0x3e,			// 00111110
	0x3e,			// 00111110 
	0x7f			// 01111111
};

void DisplayInit(void)
{
	// Either use normal display driver (slower, small memory requirement:
	u8g_InitComFn(&u8g, &u8g_dev_ssd1306_128x64_i2c, u8g_com_hw_i2c_fn);
	// or the 2x driver (faster, but more memory requirement.)
	//u8g_InitComFn(&u8g, &u8g_dev_ssd1306_128x64_2x_i2c, u8g_com_hw_i2c_fn);  
	// See the bottom of file: u8g_dev_ssd1306_128x64.c
}

void DisplayDraw(void)
{
	u8g_FirstPage(&u8g);  
	do
	{
		minutes++;
		sprintf(time, "%02d:%02d", hours, minutes);
		u8g_SetFont(&u8g, u8g_font_freedoomr10r);
		u8g_DrawStr(&u8g, 40, 20, time);
		if(minutes == 60) { minutes = 0; hours++; }
		if(hours == 24) { hours = 0; }
		
		u8g_DrawBitmapP(&u8g, 0, 0, 1, 8, rook_bitmap);
		
//		if(2 <= hours || hours <= 4) u8g_SleepOff(&u8g);
//		else u8g_SleepOn(&u8g);
		
//		u8g_DrawPixel(&u8g, pixelPosition, 25);
//		pixelPosition++;
		
//		u8g_DrawBox(&u8g, 0, 40, 127, 64);
//			u8g_DrawBitmapP(&u8g,0,10,13,19,(const uint8_t *)&img);
//			u8g_DrawFrame(&u8g, 0, 29, 127, 15);  
//			u8g.font = u8g_font_courR08;//u8g_font_profont22;  
//			u8g_DrawStr(&u8g, 10, 40, "Research Engeneer");
	} while ( u8g_NextPage(&u8g) ); 
	/* refresh screen after some delay */
	//delay_micro_seconds(10000);
}
