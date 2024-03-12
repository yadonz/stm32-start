#include "stm32f10x.h"                  // Device header
#include "RotaryEncoder.h"
#include "OLED.h"
#include "Timer.h"

extern uint32_t Count;		// 由 EXTI 旋转编码器模块控制的全局变量 Count
extern uint32_t Seconds;

int main(void)
{
	OLED_Init();
	RotaryEncoder_Init();	// 配置旋转编码器
	Timer_Init();
	
	OLED_ShowString(1, 1, "cnt:");
	OLED_ShowString(2, 1, "seconds:    s");
	
	while(1)
	{
		OLED_ShowSignedNum(1, 5, Count, 5);
		OLED_ShowNum(2, 9, Seconds, 4);
	}
}
