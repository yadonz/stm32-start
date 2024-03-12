#include "DMA.h"



void DMA_UserInit(uint32_t AddrA, uint32_t AddrB, uint32_t DMA_BufferSize)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	DMA_InitTypeDef DMA_InitStructure;
	// 外设站点的三个参数（源地址）
	DMA_InitStructure.DMA_PeripheralBaseAddr = AddrA;						// 源地址
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	// 源数据宽度
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;			// 源地址是否自增
	// 存储器站点的三个参数（目的地址）
	DMA_InitStructure.DMA_MemoryBaseAddr = AddrB;					// 同上
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	// 同上
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;			// 同上
	DMA_InitStructure.DMA_BufferSize = DMA_BufferSize;				// 缓冲区大小（CNDTR寄存器）
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;				// 定义外设为源还是目的地址
	DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;						// 0 硬件触发；1 软件触发
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;					// 配置 DMA 模式为循环还是普通模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;			// DMA 通道的优先级配置
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);					// 这里是软件触发，通道可以任意选择（当使用硬件触发时，要根据硬件的不同选择特定的通道）
	//			^ 上下都是同一个 DMA 的同一个通道
	DMA_Cmd(DMA1_Channel1, ENABLE);
}	

// 重新启动 DMA 再发生一次转运
void DMA_Transfer(uint32_t DMA_BufferSize)
{
	DMA_Cmd(DMA1_Channel1, DISABLE);				// 重写 DMA CNDTR 寄存器（缓冲区大小配置）时需要先关闭 DMA 对应的通道
	DMA_SetCurrDataCounter(DMA1_Channel1, DMA_BufferSize); // 重写 CNDTR 寄存器
	DMA_Cmd(DMA1_Channel1, DISABLE);				// 启动 DMA
	
	while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == SET);	// 阻塞，直至传输完成
	DMA_ClearFlag(DMA1_FLAG_TC1);					// 清除标志位
}