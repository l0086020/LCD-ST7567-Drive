#include "main.h"

int main(void)
{
	LCD_Init();

	while(1)
	{
		LCD_ShowString(24,8,(uint8_t *)"Hello World");
	}
}
