#include "LightSensor.h"
/*
* 将 B11 引脚配置成为上拉输入模式
*/

// 初始化光敏电阻引脚为上拉输入模式
void LightSensor_Init(void)
{
	// 打开 GPIOB 的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	// 配置 GPIOC_Pin_11 引脚为上拉输入模式
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
	
uint8_t LightSensor_Read(void)
{
	return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
}
