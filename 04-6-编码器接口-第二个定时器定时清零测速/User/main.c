/*
*	说明：
*		这里使用的是 TIM3 定时器，输出比较模块 2 （PWM 通道 2，使用外部 PA7 引脚（注意输出比较器的引脚是固定的））
*/

#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "RotaryEncoder.h"
#include "Timer.h"

extern uint16_t Period;
extern uint16_t Speed;


int main(void)
{
	// 初始化程序
	OLED_Init();
	Timer_Init();
	RotaryEncoder_Init();
	
	OLED_ShowString(1, 1, "Speed:    /s");
	
	while(1)
	{
		OLED_ShowSignedNum(1, 7, RotaryEncoder_GetData(), 3);
	}
}
