#include "Timer.h"

void Timer_Init(void)
{
	// 打开 TIM2 的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	// TIM2 时基单元的时钟源配置为外部时钟
	TIM_InternalClockConfig(TIM2);
	
	// 配置 TIM2 的时基时钟，以及重装载计数器的值
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;		// 由于这里使用的是外部时钟，信号不稳定，需要使用到滤波器滤波，这里是滤波器的分频系数（对内部时钟的分频）
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	// 计数模式为向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period = 2000 - 1;				// 计数器计数周期数 = (period + 1) 这里是2000个周期，计时周期为 1s = (prescaler + 1)/72000000 * (period + 1) = 0.5ms * (1999 + 1)
	TIM_TimeBaseInitStructure.TIM_Prescaler = 36000 - 1;			// 定时器预分频系数寄存器，预分频系数 = (prescaler + 1) 这里每个周期为 0.5ms = (35999 + 1)/72000000
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;			// 重复次数寄存器，高级定时器才有的功能，这里随便给个 0 就行
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	// 清除 TIM2 中断标志位(避免一初始化就进入 TIM2 中断)

	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	
	// TIM 中断配置
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);	// 配置 TIM2，设置为触发中断（非事件），确认配置
	
	// NVIC 配置(之前配置过 NVIC 优先级分组，不用再配置)
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;				// 配置 TIM2 的NVIC中断相关配置
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				// 确认配置生效
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	// 抢占优先级为 1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			// 响应优先级为 1
	NVIC_Init(&NVIC_InitStructure);
	
	// 启动 TIM2 
	TIM_Cmd(TIM2, ENABLE);
}
