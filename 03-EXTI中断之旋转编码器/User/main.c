#include "stm32f10x.h"                  // Device header
#include "RotaryEncoder.h"
#include "OLED.h"

extern uint32_t Count;		// 由 EXTI 旋转编码器模块控制的全局变量 Count

int main(void)
{
	OLED_Init();
	RotaryEncoder_Init();	// 配置旋转编码器
	
	
	OLED_ShowString(1, 1, "cnt:");
	
	while(1)
	{
		OLED_ShowSignedNum(1, 5, Count, 5);
	}
}
