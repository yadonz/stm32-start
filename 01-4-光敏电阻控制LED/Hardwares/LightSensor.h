#ifndef __LIGHTSENSOR_H
#define __LIGHTSENSOR_H

#include "stm32f10x.h"                  // Device header

void LightSensor_Init(void);  	// 初始化光敏传感器引脚
uint8_t LightSensor_Read(void);	// 读取引脚电平

#endif //__LIGHTSENSOR_H

