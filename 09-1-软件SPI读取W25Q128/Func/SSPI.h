#ifndef __SSPI_H
#define __SSPI_H

#include "stm32f10x.h"                  // Device header

void SSPI_Init(void);
void SSPI_Start(void);
void SSPI_Stop(void);
uint8_t SSPI_ByteExchange(uint8_t sbyte);

#endif //__SSPI_H
