#include "stm32f10x.h"                  // Device header
#include "LED.h"
#include "LightSensor.h"

void delay(uint32_t s);

int main(void) 
{
	uint8_t sign;
	sign = 0;
	
	LED_Init();
	LightSensor_Init();
	LED_OFF();
	while(1) 
	{
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, (BitAction)LightSensor_Read());
		delay(7200000*3);
//		sign = LightSensor_Read();
//		if (sign)
//		{
//			LED_ON();
//		}
//		else if (!sign)
//		{
//			LED_OFF();
//		}
	}
}


// ----------------------------- 函数定义 -----------------------------------------------------

void delay(uint32_t s)
{
	while(s --);
}
