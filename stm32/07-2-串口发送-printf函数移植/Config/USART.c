#include "USART.h"

void USART_UserInit(void)
{	// GPIO 使用
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
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
	
	USART_Cmd(USART1, ENABLE);
}


void USART_PrintString(uint8_t* str)
{
	for(; *str != '\0'; str ++)
	{
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	// 判断发送寄存器是否清空
		USART_SendData(USART1, (uint16_t)*str);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);		// 判断数据移位寄存器是否清空
	}
}

// fputc 函数为 printf 函数底层（printf函数通过把数据使用 fputc 函数一个字节一个字节的发送出去）
// 定义一个 fputc 函数，就实现了对对应的 printf 函数的移植（将 printf 函数移植到了对应的串口上）
int fputc(int ch, FILE *f)
{
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	// 判断发送寄存器是否清空
	USART_SendData(USART1, ch);
	return ch;
}
