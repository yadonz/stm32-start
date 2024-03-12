#include "VoltageResistorMeasure.h"

extern uint16_t Adc[];

void VoltageResistorMeasure_Init(void)
{
	GPIO_UserInit();
	TIM_UserInit();
	ADC_UserInit();
	DMA_UserInit();
}

