#ifndef __SERIAL_H
#define __SERIAL_H

#include "stm32f10x.h"                  // Device header
// 串口状态标志位
typedef enum{
	Serial_State_Standby = 0,
	Serial_State_Working = 1,
	Serial_State_Finished = 2
}Serial_State_TypeDef;

extern uint8_t Str_receive[];
extern Serial_State_TypeDef Serial_State;	// 串口缓冲区状态标志位
extern uint8_t Serial_Buffer[];


void Serial_UserInit(void);
void Serial_PrintChar(char c);
void Serial_PrintString(char* str);
char Serial_GetChar(void);
void Serial_GetString(void);
uint8_t* Serial_Get8bytes(void);				// 返回数据包缓冲区的内容


#endif //__SERIAL_H
