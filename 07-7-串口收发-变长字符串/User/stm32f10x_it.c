/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */



/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 
// 判断一个字符是不是待转义的字符
uint8_t isEscapeCharacter(char c)
{
	char* str = "\\@";
	for(; str != '\0'; str ++)
	{
		if(*str == c)
		{
			return 1;
		}
	}
	return 0;
}
char GetChar(void)
{
		while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);	// 阻塞直到读数据寄存器接收到数据
		return (uint8_t)USART_ReceiveData(USART1);
}
void USART1_IRQHandler(void)
{/**这里设定一种具有三种状态的状态机，分别是：
	- 待机状态：状态为 0
	- 遇到数据包头标识字节准备接受，状态为 1
	- 接收数据，并在指定字节位置判断包尾字符，同时获取字符串：成功状态为 2，表示字符串有效；失败状态变为 0，表示字符串无效
	*/
	uint8_t i;
	char c;
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		if(Serial_State == Serial_State_Standby && GetChar() == '@') // 如果待机状态时收到了 @ 字符，那么准备接收字符串序列
		{
			Serial_State = Serial_State_Working;	// 待机状态接收到包头，那么状态更改为工作状态，读取数据包内容
			for(i = 0; i < 100
			; i ++)
			{
				c = GetChar();
				if(c != '\\')	// 如果收到的字符不是转义字符开始标记，那么直接存入字符串缓冲区
				{
					Serial_Buffer[i] = c;
				}
				else			// 如果收到了转义字符开始标记，那么接受并判断下一个字符串是否是应该被转义的字符，如果不是，说明数据包出错，状态转为 standby 状态标记缓冲区无效
				{
					c = GetChar();
					if(isEscapeCharacter(c) == 1)
					{
						Serial_Buffer[i] = c;
					}
					else
					{
						break;
					}
				}
				
			}
			if(GetChar() != (char)0x00)		// 读取完数据包后，如果数据包尾不是指定标识数据
			{
				Serial_State = Serial_State_Standby;// 那么状态更改为待机状态，表示此时缓冲区数据无效，并退出
			}
			else 									// 读取完数据包后，如果数据包尾是指定标识数据
			{
				Serial_State = Serial_State_Finished;// 那么状态更改为完成，表示此时缓冲区数据有效（读取完缓冲区数据后记得修改状态为待机）
			}
		}

		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
