#ifndef __STEERINGENGINE_H
#define __STEERINGENGINE_H

#include "stm32f10x.h"                  // Device header
#include "PWM.h"


void SteeringEngine_Init(void);
void SteeringEngine_Action(uint8_t ratio);

#endif //__STEERINGENGINE_H
