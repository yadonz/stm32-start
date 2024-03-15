#include "SI2C.h"

// 软件 I2C 的端口 PB12=SDA PB13=SCL
#define SI2C_PORT 		GPIOB					
#define SI2C_RCC  		RCC_APB2Periph_GPIOB
#define SI2C_SCL_PIN	GPIO_Pin_13
#define SI2C_SDA_PIN	GPIO_Pin_12


void SI2C_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(SI2C_RCC, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = SI2C_SCL_PIN | SI2C_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SI2C_PORT, &GPIO_InitStructure);
	
}

void SI2C_SCL_W(uint8_t bit)
{
	GPIO_WriteBit(SI2C_PORT, SI2C_SCL_PIN, (BitAction)bit);
	Delay_us(10);
}

void SI2C_SDA_W(uint8_t bit)
{
	GPIO_WriteBit(SI2C_PORT, SI2C_SDA_PIN, (BitAction)bit);
	Delay_us(10);
}

uint8_t SI2C_SDA_R(void)
{
	uint8_t bit = GPIO_ReadInputDataBit(SI2C_PORT, SI2C_SDA_PIN);
	Delay_us(10);
	return bit;
}

void SI2C_Start(void)
{
	SI2C_SCL_W(1);
	SI2C_SDA_W(1);
	SI2C_SDA_W(0);
	SI2C_SCL_W(0);
}

void SI2C_Stop(void)
{
	SI2C_SDA_W(0);
	SI2C_SCL_W(1);
	SI2C_SDA_W(1);
}

uint8_t SI2C_ReceiveAck(void)
{
	SI2C_SCL_W(1);
	SI2C_SDA_W(1);
	uint8_t Ack = SI2C_SDA_R();
	SI2C_SCL_W(0);
	return Ack;
}

void SI2C_SendAck(uint8_t ack)
{
	SI2C_SDA_W(ack);
	SI2C_SCL_W(1);
	SI2C_SCL_W(0);
}

void SI2C_SendByte(uint8_t byte)
{
	for (int i = 7; i >= 0; i --)
	{
		SI2C_SDA_W((byte >> i) & 0x01);
		SI2C_SCL_W(1);
		SI2C_SCL_W(0);
	}
}

uint8_t SI2C_ReceiveByte(void)
{
	uint8_t byte = 0;
	SI2C_SDA_W(1);
	for (int i = 7; i >= 0; i --)
	{
		SI2C_SCL_W(1);
		byte |= (SI2C_SDA_R() << i);
		SI2C_SCL_W(0);
	}
	
	return byte;
}

