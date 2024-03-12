#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"                  // Device header

void LED_Init(void);	// LED 初始化函数
void LED_ON(void);		// 灯亮
void LED_OFF(void);		// 灯灭

#endif //__LED_H

