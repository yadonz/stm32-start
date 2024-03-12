#ifndef __USART_H
#define __USART_H

#include <stdio.h>
#include "stm32f10x.h"                  // Device header

void USART_UserInit(void);
void USART_PrintString(uint8_t* str);


#endif //__USART_H
