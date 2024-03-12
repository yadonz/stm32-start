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

char GetChar(void)
{
		while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);	// 阻塞直到读数据寄存器接收到数据
		return (uint8_t)USART_ReceiveData(USART1);
}
void USART1_IRQHandler(void){
	// 包头字符为 '@'；包尾字符为 '!'；转义标识字符为 '\'
	char c;
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET){
		c = GetChar();
		if(Serial_State == Serial_State_Standby){		// 待命状态
			if(c == '@'){								// 待命状态接收到字符 '@' 
				Serial_State = Serial_State_Working1;	// 转移到工作状态 1
				i = 0;									// 待命状态转换到工作状态时，要将索引清零
			}
		}
		else if(Serial_State == Serial_State_Working1){	// 工作状态 1
			if(i >= 99){								// 当接收到的字符达到 99 时
				Serial_State = Serial_State_Finished;	// 切换状态为完成（这是一个特殊情况，当输入的字符长度超过缓冲区大小时，不等待包尾字符的出现，直接截断已收到的字符，并将缓冲区内容视为有效）
			}
			else if(c == '!'){							// 遇到结束字符
				Serial_State = Serial_State_Finished;	// 转换工作状态为完成
			}
			else if(c == '\\'){							// 遇到转义字符标记字符
				Serial_State = Serial_State_Working2;	// 进入工作状态 2 处理转义字符
			}
			else if(c == '@'){							// 再次遇到 @ 字符，不合法，缓冲区数据无效
				Serial_State = Serial_State_Standby;	// 转换状态为待命状态
			}
			else{										// 遇到的为其它普通的字符
				Serial_Buffer[i] = c;					// 将普通字符加入缓冲区
				i ++;									// 缓冲区索引加一
			}
		}
		else if(Serial_State == Serial_State_Working2){	// 工作状态 2（遇到转义字符标记后）
			if(c == '!' || c == '@' || c == '\\'){		// 如果遇到合法的被转义字符
				Serial_Buffer[i] = c;					// 将合法的被转义字符存入缓冲区
				i ++;									// 缓冲区索引加一
				Serial_State = Serial_State_Working1;	// 进入工作状态 1
			}
			else{										// 被转义字符不合法
				Serial_State = Serial_State_Standby;	// 进入待命状态，缓冲区数据无效
			}
		}
		else if(Serial_State == Serial_State_Finished)	// 完成状态
		{
			Serial_Buffer[i] = '\0';					// 完成状态在字符串末尾加上字符串结束标记，此时不转移状态，这里的状态转移要等待读缓冲区函数读出数据并转移状态
		}
		
		
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
