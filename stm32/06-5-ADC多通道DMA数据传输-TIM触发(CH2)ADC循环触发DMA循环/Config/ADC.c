#include "ADC.h"


void ADC_UserInit(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);	// ADC1 时钟
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);	// 配置驱动 ADC 模块的 ADCCLK 时钟
	

	// ADC 规则组通道配置
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 2, ADC_SampleTime_55Cycles5);	
	// ADC 模块配置
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;					// ADC 连续转换模式开关
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;				// ADC 数据对齐方式开关
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_CC2;	// 选择 ADC 的外部触发模式（定时器触发）
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;					// ADC 这个参数用于选择单 ADC 还是多 ADC 模式，只有这个参数是单 ADC 模式
	ADC_InitStructure.ADC_NbrOfChannel = 2;								// ADC 配置有效的通道数（这个参数只在扫描模式下有效非扫描模式下这个参数无效）
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;						// ADC 扫描模式开关（多通道模式/单通道模式）
	ADC_Init(ADC1, &ADC_InitStructure);									// 配置 ADC1

	ADC_DMACmd(ADC1, ENABLE);		// 打开 ADC1 中的 DMA 触发开关（只有打开这个开关 ADC 才能触发 DMA）
	ADC_Cmd(ADC1, ENABLE);			// 打开 ADC1 运行开关
	
	// ADC 校准
	ADC_ResetCalibration(ADC1);							// 复位校准
	while(ADC_GetResetCalibrationStatus(ADC1) == SET);  // 等待直至校准完成
	ADC_StartCalibration(ADC1);							// 校准
	while(ADC_GetCalibrationStatus(ADC1) == SET);		// 等待直至校准完成
	
	ADC_ExternalTrigConvCmd(ADC1, ENABLE);	// 这个开挂用于决定是否由外部触发源来触发 ADC 的动作
}



