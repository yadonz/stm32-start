#include "stm32f10x.h"                  // Device header


void delay(uint32_t s)
{
	while (s --);
}

int main(void) 
{

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	while(1) 
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_13);
		delay(3600000);
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		delay(3600000);
	}
}
