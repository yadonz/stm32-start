#ifndef __ROTARYENCODER_H
#define __ROTARYENCODER_H

#include "stm32f10x.h"                  // Device header

void RotaryEncoder_Init(void);
int16_t Encoder_Get(void);



#endif //__ROTARYENCODER_H
