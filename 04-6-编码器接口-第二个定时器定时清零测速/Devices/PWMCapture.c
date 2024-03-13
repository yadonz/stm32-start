#include "PWMCapture.h"

void PWMCapture_Init(void)
{
	// PWM 使用 TIM2 捕获通道 1 ，GPIO引脚 PA0
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	// 初始化 PWM 输出引脚 PA0
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// TIM2 时基单元的时钟源配置为外部时钟
	TIM_InternalClockConfig(TIM2);
	
	// 配置 TIM2 的时基时钟，以及重装载计数器的值
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;		// 由于这里使用的是外部时钟，信号不稳定，需要使用到滤波器滤波，这里是滤波器的分频系数（对内部时钟的分频）
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	// 计数模式为向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period = 0xffff;					// 重装载寄存器值（0x01 - 0xffff）
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;				// 预分频寄存器值（0x01- 0xffff）
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;			// 重复次数寄存器，高级定时器才有的功能，这里随便给个 0 就行
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	// 配置输入捕获功能
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;				// 选择捕获通道 1
	TIM_ICInitStructure.TIM_ICFilter = 0xF;							// 捕获模式滤波器选择 采样频率 = fDTS/32，采样数量 N = 8
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; 	// 下降沿捕获模式
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;			// 捕获后的信号的分频系数（分频后进入捕获模块）
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;	// 进入捕获通道的预分频器的信号（直连信号，交叉信号，TRC信号（来自其它TIM的级联信号））
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	
	
	// 从模式配置
	TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Reset);	// TIM_SlaveMode_Reset 会在上升沿清空 CNT，“并更新定时器相关寄存器”
	// 触发源选择
	TIM_SelectInputTrigger(TIM2, TIM_TS_TI1FP1);	// 触发源是用于触发定时器开始工作的信号源
	
	// 启动 TIM3
	TIM_Cmd(TIM2, ENABLE);

}


uint16_t PWMCapture_Get(void)
{
	return (TIM_GetCapture1(TIM2));
}
