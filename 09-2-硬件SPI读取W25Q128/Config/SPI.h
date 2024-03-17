#ifndef __SPI_H
#define __SPI_H

#include "stm32f10x.h"                  // Device header

void SPI_UserInit(void);
void SPI_Start(void);
void SPI_Stop(void);
uint8_t SPI_ByteExchange(uint8_t byte);


#endif //__SPI_H
