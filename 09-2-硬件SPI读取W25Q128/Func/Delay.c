#include "Delay.h"


void Delay_us(uint32_t us)
{
	SysTick->LOAD = 72*us;
	SysTick->VAL = 0x00;
	SysTick->CTRL |= (SysTick_CTRL_CLKSOURCE | SysTick_CTRL_ENABLE);// 时钟源选择 AHB，并且打开定时器开关
	while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));			// 阻塞，直到计数时间到
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;						// 关闭定时器
}

void Delay_ms(uint16_t ms)
{
	while (ms --)
	{
		Delay_us(1000);
	}
}

void Delay_s(uint16_t s)
{
	while (s --)
	{
		Delay_ms(1000);
	}
}
