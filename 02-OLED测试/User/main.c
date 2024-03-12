#include "stm32f10x.h"                  // Device header
#include "OLED.h"

void delay(uint32_t s);

int main(void) 
{
	
	OLED_Init();
	OLED_ShowString(1, 1, "Hello");
	OLED_ShowString(2, 1, "This");
	OLED_ShowString(3, 1, "is a");
	OLED_ShowString(4, 1, "test.");
	while(1)
	{
	}
}


// ----------------------------- 函数定义 -----------------------------------------------------

void delay(uint32_t s)
{
	while(s --);
}
