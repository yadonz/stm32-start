#include "Button.h"

void Button_Init(void)
{	// 按键使用 EXTI 外部中断 2，4 号线路，GPIOA 引脚
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	// 配置引脚功能（这里使用到的是 A2, A4 两个引脚）为上拉输入模式
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_4;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// 将对应的引脚（PA2，PA4）配置为 EXTI 外设功能
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource2);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4);
	
	// 初始化 EXTI 模块
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line2 | EXTI_Line4;	// 初始化 EXTI2 和 EXTI4 号通道
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;				// 设置状态为开启
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;		// 设置触发模式为中断模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	// 设置为下降沿触发
	EXTI_Init(&EXTI_InitStructure);							// 根据以上设置配置 EXTI 中断
	
	// NVIC 配置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	// 配置 EXTI2 对应的 NVIC 设置
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	// 配置 EXTI4 对应的 NVIC 设置
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
}


