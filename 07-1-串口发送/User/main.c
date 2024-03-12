#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "USART.h"


int main(void)
{
	uint32_t t;
	
	USART_UserInit();
	USART_PrintString((uint8_t*) "Hello world!\n");

	
	while(1)
	{
		USART_PrintString("你好呀...\n");
		for(t = 720000*3; t; t --);
	}
}



