#ifndef __ROTARYENCODER_H
#define __ROTARYENCODER_H

#include "stm32f10x.h"                  // Device header

extern uint16_t Speed;

void RotaryEncoder_Init(void);
int16_t RotaryEncoder_GetData(void);



#endif //__ROTARYENCODER_H
