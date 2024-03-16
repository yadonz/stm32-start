#include "SI2C.h"
#include "Delay.h"

// Soft I2C Configration
#define RCC_SI2C RCC_APB2Periph_GPIOB
#define SI2C_PORT GPIOB
#define SDA_PIN GPIO_Pin_12
#define SCL_PIN GPIO_Pin_13

void SI2C_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_SI2C, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = SCL_PIN | SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SI2C_PORT, &GPIO_InitStructure);
	
	GPIO_SetBits(SI2C_PORT, SCL_PIN | SDA_PIN); // 初始化为 I2C IDLE 电平（SCL 和 SDA 均为高电平）
}	
// 写 SCL
void SI2C_SCL_W(uint8_t Bit) 
{
	GPIO_WriteBit(SI2C_PORT, SCL_PIN, (BitAction)Bit);
	Delay_us(10);
}
// 写 SDA
void SI2C_SDA_W(uint8_t Bit)
{
	GPIO_WriteBit(SI2C_PORT, SDA_PIN, (BitAction)Bit);
	Delay_us(10);
}
// 读 SDA
uint8_t SI2C_SDA_R(void)
{
	uint8_t bit = GPIO_ReadInputDataBit(SI2C_PORT, SDA_PIN);
	Delay_us(10);
	return bit;
}

// 起始信号
void SI2C_Start(void)
{	// 起始时序
	SI2C_SCL_W(1);	// 释放 SCL 数据线
	SI2C_SDA_W(1);	// 释放 SDA 数据线
	SI2C_SDA_W(0);	// 拉低 SDA 数据线
	SI2C_SCL_W(0);	// 拉低 SCL 时钟线
}

// 终止信号
void SI2C_Stop(void)
{// 停止时序
	SI2C_SDA_W(0);	// 拉低 SDA 数据线
	SI2C_SCL_W(1);	// 释放 SCL 数据线
	SI2C_SDA_W(1);	// 释放 SDA 数据线
}
// 发送应答位
void SI2C_SendAck(uint8_t AckBit)
{
	SI2C_SDA_W(AckBit);	// 写入应答
	SI2C_SCL_W(1);		// 释放时钟线，通知从设备读取应答位
	SI2C_SCL_W(0);		// 拉低时钟线，为发送下一位做准备
}

// 接收应答位
uint8_t SI2C_ReceiveAck(void)
{
	uint8_t bit;
	SI2C_SDA_W(1);		// 释放 SDA，给从设备应答做准备
	SI2C_SCL_W(1);		// 释放 SCL，通知从设备发送应答信息
	bit = SI2C_SDA_R();	// 读 SDA 应答
	SI2C_SCL_W(0);		// 读取结束，拉低 SCL
	return bit;
}

void SI2C_SendByte(uint8_t Byte)	// 写入一个字节
{// 写入时序
	for (int i = 7; i >= 0; i --)
	{
		SI2C_SDA_W(Byte & (0x01 << i)); // 写入数据
		SI2C_SCL_W(1);					// 维持释放时钟线一段时间
		SI2C_SCL_W(0);					// 释放时钟线，等待进入下一个脉冲
	}
	
}

uint8_t SI2C_GetByte(void)				// 读取一个字节（直接返回）
{
	uint8_t byte = 0;
	SI2C_SDA_W(1);		// 读取数据前主机一定要释放数据线的控制权
	for (int i = 7; i >= 0; i --)
	{
		SI2C_SCL_W(1);	// 释放时钟线
		byte |= (GPIO_ReadInputDataBit(SI2C_PORT, SDA_PIN) << i);
		SI2C_SCL_W(0);	// 拉低时钟线，进入下一个脉冲
	}
	return byte;
}

void SI2C_SendBytes(uint8_t DevAddr, uint8_t RegAddr, uint8_t* Bytes, uint16_t BytesLength);	// 发送一串字节（指定发送字节串的长度）
void SI2C_GetBytes(uint8_t DevAddr, uint8_t RegAddr, uint16_t BytesLength);						// 读取一串字节（指定读取字节串的长度）


