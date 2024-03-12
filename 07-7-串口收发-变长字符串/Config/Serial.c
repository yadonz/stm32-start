#include "Serial.h"



uint8_t Str_receive[32] = {0};	// 用于接收字符串的数组

void Serial_UserInit(void)
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

void Serial_PrintChar(char c)
{
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	// 判断发送寄存器是否清空
	USART_SendData(USART1, c);
}

void Serial_PrintString(char* str)
{
	for(; *str != '\0'; str ++)
	{
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	// 判断发送寄存器是否清空
		USART_SendData(USART1, (uint16_t)*str);
	}
}

char* Serial_GetString(void)				// 返回数据包缓冲区的内容（如果数据无效那么返回空指针）
{
	if(Serial_State == Serial_State_Finished)
	{
		Serial_State = Serial_State_Standby;// 读取缓冲区数据之后，状态将会转变为待命状态，记得修改状态
		return Serial_Buffer;
	}
	return (char*)0x00000000;
}
