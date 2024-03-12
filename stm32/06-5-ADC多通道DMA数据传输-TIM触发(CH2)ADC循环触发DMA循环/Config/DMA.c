#include "DMA.h"

uint16_t Adc[2] = {0};

void DMA_UserInit(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);		// DMA1 时钟
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;			// 源地址 <
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	// 源数据宽度 <
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		// 源地址是否自增 <
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Adc;					// 目的地址 <
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;		// 目的地址宽度 <
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					// 目的地址是否自增 <
	DMA_InitStructure.DMA_BufferSize = 2;									// 缓冲区大小（CNDTR寄存器）
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;						// 定义外设为源还是目的地址
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;							// 0 硬件触发；1 软件触发
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;							// 配置 DMA 模式为循环还是普通模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;					// DMA 通道的优先级配置
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);							// > ADC1 触发，注意 DMA 触发通道选择（ADC1 对应 DMA 触发通道 1）
	
	DMA_Cmd(DMA1_Channel1, ENABLE);	// 打开 DMA1 的 通道 1 启动开关
}



