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
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	// 判断发送寄存器是否清空
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

// 可变参数；使用 vsprintf 函数来封装一个可变参数函数
void USART_Printf(char* format, ...)
{
	char str[100];
	va_list arg_list;
	va_start(arg_list, format);		// 注意函数的参数位置
	vsprintf(str, format, arg_list);// 注意函数的参数位置
	va_end(arg_list);				// 记得释放参数列表
	// 获取到字符串之后使用自定义的 USART 函数输出字符串即可
	USART_PrintString((uint8_t*)str);
}

