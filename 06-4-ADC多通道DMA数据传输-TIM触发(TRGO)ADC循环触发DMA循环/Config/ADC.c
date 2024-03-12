#include "ADC.h"

uint16_t Adc[2] = {0};

void AD_Init(void)
{	// 这里是使用到的是 ADC1 GPIOB_Pin0
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	// TIM 时基单元时钟配置位外部时钟模式
	TIM_InternalClockConfig(TIM3);
	// ADC 时基时钟
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	// 将 GPIOB_Pin0 配置成模拟输入的模式
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	// 配置 TIM 时基配置
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 36000 - 1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 2000 - 1;
//	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = ;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);	// 去除 TIM_TimeBaseInit 最后一行代码会触发定时器中断的副作用
	
	// （TRGO出发不需要配置）配置产生 PWM 波配置寄存器(也即捕获/比较模块，使用通道 1 配置函数)
//	TIM_OCInitTypeDef TIM_OCInitStructure;
//	TIM_OCStructInit(&TIM_OCInitStructure);
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;				// PWM 输出模式1
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		// PWM 输出的极性（有效电平为高电平还是低电平）
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	// 使能 OC 状态
//	TIM_OCInitStructure.TIM_Pulse = 0;								// CCR 寄存器的值，也即有效电平的时长（硬件支持取值 0x0000 - 0xffff，舵机控制在这里可以取值 25 ~ 125）
//	TIM_OC1Init(TIM3, &TIM_OCInitStructure);						// >>>>>> 注意使用的通道(不同初始化函数配置不同通道)与引脚是否对应 <<<<<<<
	
	TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);			// 选择 TIM3 触发外部事件的主模式
	
	// ADC 规则组通道配置
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 2, ADC_SampleTime_55Cycles5);	
	// ADC 模块配置
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;					// 连续转换模式开关
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;				// 数据对齐方式开关
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;	// 选择 ADC 的外部触发模式
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;					// 这个参数用于选择单 ADC 还是多 ADC 模式，只有这个参数是单 ADC 模式
	ADC_InitStructure.ADC_NbrOfChannel = 2;								// 配置有效的通道数（这个参数只在扫描模式下有效非扫描模式下这个参数无效）
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;						// 扫描模式开关（多通道模式/单通道模式）
	ADC_Init(ADC1, &ADC_InitStructure);


	DMA_InitTypeDef DMA_InitStructure;
	// 外设站点的三个参数（源地址）
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;			// 源地址 <
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	// 源数据宽度 <
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		// 源地址是否自增 <
	// 存储器站点的三个参数（目的地址）
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Adc;			// 同上 <
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	// 同上
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;			// 同上
	DMA_InitStructure.DMA_BufferSize = 2;							// 缓冲区大小（CNDTR寄存器） <
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;				// 定义外设为源还是目的地址
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;					// 0 硬件触发；1 软件触发 >>>>>> 设置为硬件触发 <<<<<<
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;					// 配置 DMA 模式为循环还是普通模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;			// DMA 通道的优先级配置
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);					// > ADC1 触发，注意 DMA 触发通道选择（ADC1 对应 DMA 触发通道 1）
	//			^ 上下都是同一个 DMA 的同一个通道（同一个通道的其它硬件触发信号也可以触发该通道，且不可配置）
	
	DMA_Cmd(DMA1_Channel1, ENABLE);
	// !!!!!!!! 打开 ADC 的 DMA模式 开关 !!!!!!!!
	ADC_DMACmd(ADC1, ENABLE);
	// 启动 ADC1
	ADC_Cmd(ADC1, ENABLE);
	// 启动 TIM3
	TIM_Cmd(TIM3, ENABLE);
	
	// ADC 校准
	ADC_ResetCalibration(ADC1);							// 复位校准
	while(ADC_GetResetCalibrationStatus(ADC1) == SET);  // 等待直至校准完成
	ADC_StartCalibration(ADC1);							// 校准
	while(ADC_GetCalibrationStatus(ADC1) == SET);		// 等待直至校准完成
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	// 启动 ADC
}

/**
  * @brief 获取 ADC 的值
  * @param None
  * @retVal 返回 ADC 的值
  */
// 不需要函数触发 ADC
//void AD_GetValue(void)
//{	// 配置需要转运的数据缓冲区大小（ADC 扫描的通道数）
//	DMA_Cmd(DMA1_Channel1, DISABLE);
//	DMA_SetCurrDataCounter(DMA1_Channel1, 2);
//	DMA_Cmd(DMA1_Channel1, ENABLE);
//	
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	// 启动 ADC
//	
//	while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);	// 阻塞，直至传输完成
//	DMA_ClearFlag(DMA1_FLAG_TC1);						// 清除标志位
//}


