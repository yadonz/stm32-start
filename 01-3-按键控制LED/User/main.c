#include "stm32f10x.h"                  // Device header

void userInit(void);
void delay(uint32_t s);
void button_touch(uint32_t* sign);

int main(void) 
{
	uint32_t sign;
	sign = 0;
	userInit();	// 初始化 GPIO 引脚
	while(1) 
	{
		button_touch(&sign);
		if (sign)
		{
			GPIO_SetBits(GPIOC, GPIO_Pin_13);
			delay(360000*3);
		}
		else if (!sign)
		{
			GPIO_ResetBits(GPIOC, GPIO_Pin_13);
			delay(360000*3);
		}
	}
}


// ----------------------------- 函数定义 -----------------------------------------------------
void userInit(void)
{
	// 打开 GPIOA 和 GPIOC 的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA, ENABLE);
	// 配置 GPIOC_Pin_13 引脚为推挽输出功能，速度为 50 MHz
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	// 配置 GPIOA_Pin_6 引脚为上拉输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void delay(uint32_t s)
{
	while (s --);
}

void button_touch(uint32_t* sign)
{
	if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6))
	{
		delay(360000*3);
		if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6))
		{
			*sign = !(*sign);
		}
	}
}
