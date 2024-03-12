#ifndef __ADC_H
#define __ADC_H

#include "stm32f10x.h"                  // Device header

void AD_Init(void);
uint16_t AD_GetValue(uint8_t ADC_channels);
#endif //__ADC_H

