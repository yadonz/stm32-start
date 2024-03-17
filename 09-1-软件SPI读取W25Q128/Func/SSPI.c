#include "SSPI.h"


#define SSPI_RCC  		RCC_APB2Periph_GPIOA
#define SSPI_ClockCmd 	RCC_APB2PeriphClockCmd
#define SSPI_PORT 		GPIOA
#define SSPI_PORT_Init	GPIO_Init
#define SSPI_PIN_MISO	GPIO_Pin_6
#define SSPI_PIN_MOSI	GPIO_Pin_7
#define SSPI_PIN_CLK	GPIO_Pin_5
#define SSPI_PIN_CS		GPIO_Pin_4


void SSPI_W_SS(uint8_t bit)
{
	GPIO_WriteBit(SSPI_PORT, SSPI_PIN_CS, (BitAction)bit);
}

void SSPI_W_CLK(uint8_t bit)
{
	GPIO_WriteBit(SSPI_PORT, SSPI_PIN_CLK, (BitAction)bit);
}

void SSPI_W_MOSI(uint8_t bit)
{
	GPIO_WriteBit(SSPI_PORT, SSPI_PIN_MOSI, (BitAction)bit);
}

uint8_t SSPI_R_MISO(void)
{
	return GPIO_ReadInputDataBit(SSPI_PORT, SSPI_PIN_MISO);
}

void SSPI_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	SSPI_ClockCmd(SSPI_RCC, ENABLE);
	
	// 输入引脚配置为上拉输入，其余引脚配置为推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = SSPI_PIN_MISO;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	SSPI_PORT_Init(SSPI_PORT, &GPIO_InitStructure);
	
	// 除输入引脚外全部配置为推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = SSPI_PIN_CLK | SSPI_PIN_CS | SSPI_PIN_MOSI;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	SSPI_PORT_Init(SSPI_PORT, &GPIO_InitStructure);
	
	// 初始化待机模式
	SSPI_W_CLK(0);	// 模式一，IDLE 状态是低电平
	SSPI_W_SS(1);	// 待机状态设备未被选中
}

void SSPI_Start(void)
{
	SSPI_W_SS(0);
}

void SSPI_Stop(void)
{
	SSPI_W_SS(1);
}

uint8_t SSPI_ByteExchange(uint8_t sbyte)
{
	uint8_t rbyte = 0;
	for (int i = 7; i >= 0; i --)
	{
		SSPI_W_MOSI((sbyte >> i) & 0x01);	// 模式一要提前把数据放到 MOSI 端口，等时钟上升沿读取数据
		SSPI_W_CLK(1);						// 时钟沿上升，预备采样数据
		rbyte |= (SSPI_R_MISO()  << i);		// 采样数据
		SSPI_W_CLK(0);						// 时钟沿下降，
	}
	return rbyte;
}
