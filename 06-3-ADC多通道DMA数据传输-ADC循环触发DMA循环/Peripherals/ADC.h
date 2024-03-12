#ifndef __ADC_H
#define __ADC_H

#include "stm32f10x.h"                  // Device header

extern uint16_t Adc[2];

void AD_Init(void);
//void AD_GetValue(void);

#endif //__ADC_H

