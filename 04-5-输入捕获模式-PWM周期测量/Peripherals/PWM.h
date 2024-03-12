#ifndef __PWM_H
#define __PWM_H

#include "stm32f10x.h"                  // Device header

void PWM_Init(uint16_t ARR, uint16_t PSC);
void PWM_DutyRatio(uint16_t compare);
void PWM_Period_halfms(uint16_t Freq);

#endif //__PWM_H
