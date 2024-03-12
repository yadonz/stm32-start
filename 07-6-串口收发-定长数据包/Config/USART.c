#include "USART.h"

uint8_t Str_receive[32] = {0};	// 用于接收字符串的数组

void USART_UserInit(void)
{	// GPIO 使用
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	// 打开 GPIOA 时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	// 打开 USART1 RCC 时钟
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART1, ENABLE);
}

void USART_PrintChar(char c)
{
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	// 判断发送寄存器是否清空
	USART_SendData(USART1, c);
}

void USART_PrintString(char* str)
{
	for(; *str != '\0'; str ++)
	{
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	// 判断发送寄存器是否清空
		USART_SendData(USART1, (uint16_t)*str);
	}
}

char USART_GetChar(void)
{
		while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);	// 阻塞直到读数据寄存器接收到数据
		return (uint8_t)USART_ReceiveData(USART1);
}

void USART_GetString(void)
{
	uint8_t c;
	uint16_t i;
	
	while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);	// 阻塞直到读数据寄存器接收到数据
	c = (uint8_t)USART_ReceiveData(USART1);
	i = 0;
	for(i = 0; (i < 32) && (c != '\0') ; i ++ )	// 注意，这里的条件判断 c != '\0' 永远都不会执行，因为 PC 发送过去的字符串不是以 '\0' 结尾的（所以这段代码会等待 32 个字节全部填充完毕才会执行）
	{
		Str_receive[i] = c;
		while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);	// 阻塞直到读数据寄存器接收到数据
		c = (uint8_t)USART_ReceiveData(USART1);
	}
	Str_receive[i] = '\0';
}
