#include "GPIO.h"

void GPIO_UserInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	// GPIOB 时钟（此处为无效项）
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;			// 模拟输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;	// 引脚 0 和 引脚 1
	GPIO_Init(GPIOB, &GPIO_InitStructure);					// 配置 GPIOB
}
