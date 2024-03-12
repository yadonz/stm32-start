/*
*	说明：
*		这里使用的是 TIM3 定时器，输出比较模块 2 （PWM 通道 2，使用外部 PA7 引脚（注意输出比较器的引脚是固定的））
*/

#include "stm32f10x.h"                  // Device header
#include "RotaryEncoder.h"
#include "OLED.h"
#include "SteeringEngine.h"

extern uint32_t Angle;

int main(void)
{
	uint32_t cnt;
	OLED_Init();
	RotaryEncoder_Init();	// 配置旋转编码器
	SteeringEngine_Init();
	
	
	OLED_ShowString(1, 1, "AnglePer:    ");
//	PWM_DutyRatio(500);
	
	while(1)
	{
		OLED_ShowNum(1, 10, Angle, 3);
		SteeringEngine_Action(Angle);
		for(cnt = 0; cnt < 36000; cnt ++);	// 暂停一段时间
	}
}
