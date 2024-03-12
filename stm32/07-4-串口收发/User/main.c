#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "USART.h"

extern uint8_t Str_receive[];

int main(void)
{
	uint32_t t;
	
	OLED_Init();
	USART_UserInit();
	
	while(1)
	{
		USART_PrintChar(USART_GetChar());
//		for(t = 720000*3; t; t --);
	}
}



