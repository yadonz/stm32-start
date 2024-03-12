#ifndef __PWMCAPTURE_H
#define __PWMCAPTURE_H

#include "stm32f10x.h"                  // Device header

void PWMCapture_Init(void);
uint16_t PWMCapture_Get(void);


#endif //__PWMCAPTURE_H
