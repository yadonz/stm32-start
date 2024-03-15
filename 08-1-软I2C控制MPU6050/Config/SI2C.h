#ifndef __SI2C_H
#define __SI2C_H

#include "stm32f10x.h"                  // Device header



void SI2C_Init(void);
void SI2C_Start(void);
void SI2C_Stop(void);
void SI2C_SendAck(uint8_t AckBit);
uint8_t SI2C_ReceiveAck(void);
void SI2C_SendByte(uint8_t Byte);
uint8_t SI2C_GetByte(void);
void SI2C_SendBytes(uint8_t DevAddr, uint8_t RegAddr, uint8_t* Bytes, uint16_t BytesLength);	// 发送一串字节（指定发送字节串的长度）
void SI2C_GetBytes(uint8_t DevAddr, uint8_t RegAddr, uint16_t BytesLength);						// 读取一串字节（指定读取字节串的长度）

#endif //__SI2C_H

