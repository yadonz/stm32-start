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
#include "OLED.h"
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

uint16_t Period = 0;
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
  * @brief 这是编码器中断 0 号线路中断函数
  */ 
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) == SET)	// 进中断时要确认是否触发中断
	{
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == RESET)
		{
			if(Period < 1000)
			{
				Period += 1;
			}
			else
			{
				Period = 0;
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line0);	// 退出中断前要记得清除中断标志位（不然无法退出中断）
	}
}

/**
  * @brief 这是编码器中断 1 号线路中断函数
  */ 
void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line1) == SET)	// 进中断时要确认是否触发中断
	{
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == RESET)
		{
			if(Period > 0)
			{
				Period -= 1;
			}
			else
			{
				Period = 1000;
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line1);	// 退出中断前要记得清除中断标志位（不然无法退出中断）
	}
}

/**
  * @brief 这是编码器中断 1 号线路中断函数
  */ 
void EXTI2_IRQHandler(void)
{
	uint16_t cnt;
	if(EXTI_GetITStatus(EXTI_Line2) == SET)					// 进中断时要确认是否触发中断
	{
		for(cnt = 0xffff; cnt; cnt --);							// 软件消抖
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) == RESET) // 软件消抖
		{
			if(Period < 1000)
			{
				Period += 1;
			}
			else
			{
				Period = 0;
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line2);	// 退出中断前要记得清除中断标志位（不然无法退出中断）
	}
}

/**
  * @brief 这是编码器中断 1 号线路中断函数
  */ 
void EXTI4_IRQHandler(void)
{
	uint16_t cnt;
	if(EXTI_GetITStatus(EXTI_Line4) == SET)					// 进中断时要确认是否触发中断
	{
		for(cnt = 0xffff; cnt; cnt --);							// 软件消抖
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == RESET) // 软件消抖
		{
			if(Period > 0)
			{
				Period -= 1;
			}
			else
			{
				Period = 1000;
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line4);	// 退出中断前要记得清除中断标志位（不然无法退出中断）
	}
}


void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update))
	{
		Speed = TIM_GetCounter(TIM2);
		TIM_SetCounter(TIM2, 0);
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
