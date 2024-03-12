/*
*	说明：
*		这里使用的是 TIM3 定时器，输出比较模块 2 （PWM 通道 2，使用外部 PA7 引脚（注意输出比较器的引脚是固定的））
*/

#include "stm32f10x.h"                  // Device header
#include "RotaryEncoder.h"
#include "OLED.h"
#include "PWM.h"
#include "PWMCapture.h"
#include "Button.h"

extern uint16_t Period;

int main(void)
{
	uint32_t cnt;
	// 初始化程序
	OLED_Init();
	Button_Init();
	RotaryEncoder_Init();
	// PWM 发生配置
	PWM_Init(36, 1);		// PWM 波周期设置为 1us
	PWM_DutyRatio(18);		// 占空比设置为 50%
	// IC 配置
	PWMCapture_Init();
	
	OLED_ShowString(1, 1, "period:    us");
	OLED_ShowString(2, 1, "Getprd:    us");
	
	
	while(1)
	{
		PWM_Period_halfms(Period*2);
		OLED_ShowNum(1, 8, Period, 4);
		OLED_ShowNum(2, 8, PWMCapture_Get(), 4);
		for(cnt = 0; cnt < 36000; cnt ++);	// 暂停一段时间
	}
}
