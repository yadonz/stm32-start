#include "RotaryEncoder.h"
#include "stm32f10x.h"                  // Device header
#include "OLED.h"



void RotaryEncoder_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	// 如果外部没有上拉电阻时，应选择上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 0xffff;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 0x00;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1 | TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 10;
	//	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;	// TIM_EncoderInterfaceConfig 还会再配置这个参数，这里可以不配置这个参数
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	
	TIM_EncoderInterfaceConfig(TIM2, 
			TIM_EncoderMode_TI12, 		// TI1 和 TI2 每个动作触发两次，TI12 每个动作触发四次计数
			TIM_ICPolarity_Falling, 	// 通过配置极性，可以决定旋转编码器时，计数器的技术方向
			TIM_ICPolarity_Rising);		// 同上
			
	TIM_Cmd(TIM2, ENABLE);	// 打开定时器开关
}

int16_t RotaryEncoder_GetData(void)
{
	return Speed;
}
