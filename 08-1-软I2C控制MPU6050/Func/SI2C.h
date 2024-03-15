#ifndef __SI2C_H
#define __SI2C_H

#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void SI2C_Init(void);
void SI2C_Start(void);
void SI2C_Stop(void);
void SI2C_SendAck(uint8_t ack);
uint8_t SI2C_ReceiveAck(void);
void SI2C_SendByte(uint8_t byte);
uint8_t SI2C_ReceiveByte(void);

#endif //__SI2C_H
