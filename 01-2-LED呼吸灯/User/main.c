#include "stm32f10x.h"                  // Device header


void delay(uint32_t s)
{
	while (s --);
}

void lightLEDms(uint32_t ms)
{
	GPIO_SetBits(GPIOC, GPIO_Pin_13);
	delay(36*ms);
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
	delay(36*(1000 - ms));
} 

int main(void) 
{
	uint32_t i;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	while(1) 
	{
		for(i = 0; i < 1000; i += 2)
		{
			lightLEDms(i);
		}
		for(i = 1000; i; i -= 2)
		{
			lightLEDms(i);
		}
	}
}
