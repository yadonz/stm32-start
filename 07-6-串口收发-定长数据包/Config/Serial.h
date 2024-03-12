#ifndef __SERIAL_H
#define __SERIAL_H

#include "stm32f10x.h"                  // Device header
// 串口状态标志位
typedef enum{
	Serial_State_Standby = 0,
	Serial_State_Working1 = 1,
	Serial_State_Working2 = 2,
	Serial_State_Finished = 3
}Serial_State_TypeDef;

extern uint8_t Str_receive[];
extern Serial_State_TypeDef Serial_State;	// 串口缓冲区状态标志位
extern char Serial_Buffer[];


void Serial_UserInit(void);
void Serial_PrintChar(char c);
void Serial_PrintString(char* str);
char Serial_GetChar(void);
char* Serial_GetString(void);


#endif //__SERIAL_H
