#include "SteeringEngine.h"

void SteeringEngine_Init(void)
{	// 舵机的通信协议是周期 20ms，占空比位于 0.5 ~ 0.25ms 之间的 PWM 波
	PWM_Init(1000, 1440);	
}

void SteeringEngine_Action(uint8_t ratio)
{
	PWM_DutyRatio(ratio + 25);
}
