#include "stm32f10x.h"                  // Device header

void Timer_Init(void)
{
	// --------------------------------TIM2 配置---------------------------------------------
	//	配置 TIM2 为 外部时钟源模式 2
	// 打开 TIM2 的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	// （由于外部时钟用到 PA0 引脚）配置 GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	// 打开 GPIOA 时钟
	GPIO_InitTypeDef GPIO_InitStructure;					
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			// 配置为上拉输入模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				// 配置引脚 0
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
//	// TIM2 时基单元的时钟源配置为内部时钟
//	TIM_InternalClockConfig(TIM2);
	// TIM2 的时基单元的时钟源配置为外部时钟
	TIM_ETRClockMode2Config(TIM2, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_Inverted, 0x00);
	
	// 配置 TIM2 的时基时钟，以及重装载计数器的值
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;		// 由于这里使用的是外部时钟，信号不稳定，需要使用到滤波器滤波，这里是滤波器的分频系数（对内部时钟的分频）
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	// 计数模式为向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period = 10 - 1;				
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;			
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;			// 重复次数寄存器，高级定时器才有的功能，这里随便给个 0 就行
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	// 清除 TIM2 中断标志位(避免TIM2 一初始化就进入 TIM2 中断)
	// 由于 TIM_TimeBaseInit 函数的最后一部分有这样一段代码（操作）
	// 		/* Generate an update event to reload the Prescaler and the Repetition counter
    // 		values immediately */
    // 		TIMx->EGR = TIM_PSCReloadMode_Immediate;  
	// 这个操作会产生三个作用：
	//  1.预分频寄存器的值立刻写入它的影子寄存器中
	//  2.重新初始化预分频器/计数器/重复计数器中的值
	//  3.置位中断寄存器标志位，并触发 中断 或者 DMA请求（事件）
	// 这里通过清除 TIM2 的中断标志位来避免进入中断(也就是说，这行代码除掉了TIMx->EGR = TIM_PSCReloadMode_Immediate; 会进入中断的副作用 )
	// [参考参考手册的 288 页]
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
	
	// --------------------------------TIM3 配置---------------------------------------------
	//  配置 TIM3 为内部时钟源
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	TIM_InternalClockConfig(TIM3);									// TIM3 配置为内部时钟源

	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;		// 由于这里使用的是外部时钟，信号不稳定，需要使用到滤波器滤波，这里是滤波器的分频系数（对内部时钟的分频）
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	// 计数模式为向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period = 1000 - 1;				
	TIM_TimeBaseInitStructure.TIM_Prescaler = 36000 - 1;			
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;			// 重复次数寄存器，高级定时器才有的功能，这里随便给个 0 就行
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

	TIM_ClearFlag(TIM3, TIM_FLAG_Update);						// 避免 TIM_TimerBaseInit 完成后设置中断标志位的效果
	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);					// 中断允许标志

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;				// 配置 TIM3 的NVIC中断相关配置
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				// 确认配置生效
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	// 抢占优先级为 1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			// 响应优先级为 1
	NVIC_Init(&NVIC_InitStructure);
	// 配置 GPIOA1 引脚为推挽输出模式
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	TIM_Cmd(TIM3, ENABLE);	// 启动 TIM3
}

// 读取 TIM2->CNT 的值
uint16_t Get_CNT(void)
{
	return TIM_GetCounter(TIM2);
}

