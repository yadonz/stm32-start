#include "RotaryEncoder.h"
#include "stm32f10x.h"                  // Device header
#include "OLED.h"


void RotaryEncoder_Init(void)
{	/*
	 *	旋转编码器使用的是 EXTI 中断，EXTI 初始化使用到的外设有：
	 * 		- RCC 时钟配置
	 * 		- GPIO 端口配置
	 *		- AFIO 复用引脚配置
	 *		- EXTI 中断配置
	 *		- NVIC 嵌入式中断向量寄存器配置
	 */
	
	// 打开 GPIOB 端口与 AFIO 复用引脚时钟（EXTI不需要开启时钟，NVIC 作为内核外设也不需要开启时钟）
	// RCC 只能配置内核外部设备的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	// 配置引脚功能（这里使用到的是 B0, B1 两个引脚）为上拉输入模式
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// 将对应的引脚（PB0，PB1）配置为 EXTI 外设功能
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);
	
	// 初始化 EXTI 模块
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line0 | EXTI_Line1;	// 初始化EXTI0和EXTI1号线路
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;				// 设置状态为开启
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;		// 设置触发模式为中断模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	// 设置为下降沿触发
	EXTI_Init(&EXTI_InitStructure);							// 根据以上设置配置 EXTI 中断
	
	// NVIC 配置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	// 配置 EXTI0 对应的 NVIC 设置
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	// 配置 EXTI1 对应的 NVIC 设置
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
}

