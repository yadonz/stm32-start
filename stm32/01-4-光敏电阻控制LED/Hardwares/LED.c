#include "LED.h"

// LED 初始化函数
void LED_Init(void)	
{
	// 打开 GPIOC 的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	// 配置 GPIOC_Pin_13 引脚为推挽输出功能，速度为 50 MHz
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
// 灯亮
void LED_ON(void)		
{
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
}
// 灯灭
void LED_OFF(void)		
{
	GPIO_SetBits(GPIOC, GPIO_Pin_13);
}
