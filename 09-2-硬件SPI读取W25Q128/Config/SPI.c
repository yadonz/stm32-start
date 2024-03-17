#include "SPI.h"

#define SPI_PORT_RCC  	RCC_APB2Periph_GPIOA
#define SPI_PIN_PORT  	GPIOA
#define SPI_PIN_CS		GPIO_Pin_4
#define SPI_PIN_CK		GPIO_Pin_5
#define SPI_PIN_MO		GPIO_Pin_7
#define SPI_PIN_MI		GPIO_Pin_6

void SPI_UserInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	
	RCC_APB2PeriphClockCmd(SPI_PORT_RCC | RCC_APB2Periph_SPI1, ENABLE);
	
	// CS 片选引脚由软件控制，通用推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = SPI_PIN_CS;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_PIN_PORT, &GPIO_InitStructure);
	// CLK 和 MOSI 配置为复用推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = SPI_PIN_CK | SPI_PIN_MO;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_PIN_PORT, &GPIO_InitStructure);
	// MISO 配置为上拉输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = SPI_PIN_MI;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_PIN_PORT, &GPIO_InitStructure);
	
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
//	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_Init(SPI1, &SPI_InitStructure);
	
	SPI_Cmd(SPI1, ENABLE);
	
	GPIO_WriteBit(SPI_PIN_PORT, SPI_PIN_CS, Bit_SET);
}	

void SPI_Start(void)
{
	GPIO_WriteBit(SPI_PIN_PORT, SPI_PIN_CS, Bit_RESET);
}

void SPI_Stop(void)
{
	GPIO_WriteBit(SPI_PIN_PORT, SPI_PIN_CS, Bit_SET);
}	

uint8_t SPI_ByteExchange(uint8_t sbyte)
{
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1, sbyte);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	uint8_t rbyte = SPI_I2S_ReceiveData(SPI1);
	return rbyte;
}


