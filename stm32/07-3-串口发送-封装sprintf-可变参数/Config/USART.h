#ifndef __USART_H
#define __USART_H

#include <stdio.h>
#include "stm32f10x.h"                  // Device header
#include <stdarg.h>


void USART_UserInit(void);
void USART_PrintString(uint8_t* str);
void USART_Printf(char* format, ...);

#endif //__USART_H
