#include "PWM.h"
/**
  * @brief 这是 PWM 初始化函数
  * @param ARR: 这是重装载寄存器的初始值
  *	@param PSC: 这是预分频器的初始值
  *	@retval None
  */
void PWM_Init(uint16_t ARR, uint16_t PSC)
{
	// PWM 使用 TIM3，PWM通道2，PA7 引脚 输出
	// 打开 TIM3 的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	// 打开 GPIOA 时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	// 初始化 PWM 输出引脚
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  	// 注意这里是复用推挽输出，此时 GPIO 的推挽输出控制将会断开，AFIO 复用外设将会接管 GPIO 的推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;			// 配置 TIM3_CH2 通道使用的引脚 PA7
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// TIM3 时基单元的时钟源配置为外部时钟
	TIM_InternalClockConfig(TIM3);
	
	// 配置 TIM3 的时基时钟，以及重装载计数器的值
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;		// 由于这里使用的是外部时钟，信号不稳定，需要使用到滤波器滤波，这里是滤波器的分频系数（对内部时钟的分频）
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	// 计数模式为向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period = ARR - 1;					// 重装载寄存器值
	TIM_TimeBaseInitStructure.TIM_Prescaler = PSC - 1;				// 预分频寄存器值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;			// 重复次数寄存器，高级定时器才有的功能，这里随便给个 0 就行
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	// 配置产生 PWM 波配置寄存器(使用通道 1 配置函数)
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;				// PWM 输出模式1
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		// PWM 输出的极性（有效电平为高电平还是低电平）
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	// 使能 OC 状态
	TIM_OCInitStructure.TIM_Pulse = 0;								// CCR 寄存器的值，也即有效电平的时长（硬件支持取值 0x0000 - 0xffff，舵机控制在这里可以取值 25 ~ 125）
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);						// >>>>>> 注意使用的通道(不同初始化函数配置不同通道)与引脚是否对应 <<<<<<<
	
	// PWM 不用配置中断
	
	// 启动 TIM3
	TIM_Cmd(TIM3, ENABLE);
}

void PWM_DutyRatio(uint16_t compare)
{
	TIM_SetCompare2(TIM3, compare);
}

void PWM_Period_halfms(uint16_t Period)
{
	TIM_PrescalerConfig(TIM3, Period, TIM_PSCReloadMode_Immediate);
}
