#include "TIM.h"

void TIM_UserInit(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	// TIM2 时钟
	
	TIM_InternalClockConfig(TIM2);		// TIM2 时基单元的时钟配置外外部时钟模式
	
	//	TIM_TimeBaseInitStructure.TIM_ClockDivision = ;				// TIM 输入比较的滤波器时钟配置项（此处为无效项）
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	// TIM 为向上计数模式	
	TIM_TimeBaseInitStructure.TIM_Period = 36000 - 1;				// 重加载计数器
	TIM_TimeBaseInitStructure.TIM_Prescaler = 2000 - 1;				// 预分频器
//	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = ;				// 重复次数寄存器（高级定时器的功能，此处为无效项）
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);				// 配置 TIM2 
	
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;				// 输出捕获模式为 PWM 模式 1
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;		// 输出捕获模式 PWM 输出的极性（有效电平：高/低）
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	// 输出捕获模式的配置生效
	TIM_OCInitStructure.TIM_Pulse = 18000;							// CCR 寄存器的值，也即有效电平的时长（占空比设置）
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);						// 配置 TIM2 （注意，！！！！不同的通道使用的配置函数是不同的！！！！）

	TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_OC2Ref);			// 选择输出捕获模式的 TIM2_CH2 作为外部触发
	TIM_Cmd(TIM2, ENABLE);			// 打开 TIM2 运行开关	
	
}

