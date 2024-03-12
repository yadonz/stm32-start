#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"                  // Device header

extern uint8_t Str_receive[];

void USART_UserInit(void);
void USART_PrintChar(char c);
void USART_PrintString(char* str);
char USART_GetChar(void);
void USART_GetString(void);

#endif //__USART_H
