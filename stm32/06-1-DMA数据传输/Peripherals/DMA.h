#ifndef __DMA_H
#define __DMA_H

#include "stm32f10x.h"                  // Device header

void DMA_UserInit(uint32_t AddrA, uint32_t AddrB, uint32_t DMA_BufferSize);
void DMA_Transfer(uint32_t DMA_BufferSize);

#endif //__DMA_H
