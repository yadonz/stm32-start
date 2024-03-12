#include "ADC.h"

void AD_Init(void)
{	// 这里是使用到的是 ADC1 GPIOB_Pin0
	// 开启 GPIO 和 ADC 的 RCC 时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_ADC1, ENABLE);
	// 同时在 RCC 时钟里配置 ADC 模块的分频系数为 6 分频（频率 72MHz/6 = 12MHz）
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	// 将 GPIOB_Pin0 配置成模拟输入的模式
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// ADC 规则组配置（ADC1 的通道 0 和 规则组序列中的第一个位置匹配）
	// 如果有多个通道需要采样，可以继续使用这个函数配置其它通道
	ADC_RegularChannelConfig(
			ADC1, 						// 待配置的 ADC 模块
			ADC_Channel_8, 				// ADC 的通道（配置引脚时要用到）
			1, 							// 规则组中的位置
			ADC_SampleTime_55Cycles5);	// 采样保持时间（总时间 = 采样保持时间 + 转换时间）
	
	// ADC 模块配置
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;					// 连续转换模式开关
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;				// 数据对齐方式开关
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	// 选择 ADC 的外部触发模式，这里使用软件触发，配置为 None
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;					// 这个参数用于选择单 ADC 还是多 ADC 模式，只有这个参数是单 ADC 模式
	ADC_InitStructure.ADC_NbrOfChannel = 1;								// 配置有效的通道数（这个参数只在扫描模式下有效非扫描模式下这个参数无效）
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;						// 扫描模式开关（多通道模式/单通道模式）
	ADC_Init(ADC1, &ADC_InitStructure);
	
	// 中断和模拟看门狗配置（如果需要的话，这里不配置）
	
	// 启动 ADC1
	ADC_Cmd(ADC1, ENABLE);
	
	// ADC 校准
	ADC_ResetCalibration(ADC1);							// 复位校准
	while(ADC_GetResetCalibrationStatus(ADC1) == SET);  // 等待直至校准完成
	ADC_StartCalibration(ADC1);							// 校准
	while(ADC_GetCalibrationStatus(ADC1) == SET);		// 等待直至校准完成
}

/**
  * @brief 获取 ADC 的值
  * @param None
  * @retVal 返回 ADC 的值
  */
uint16_t AD_GetValue(void)
{
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);					// 软件触发一次转换
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);	// 等待直至转换完成
	return ADC_GetConversionValue(ADC1);					// 获取 ADC 值
	
}


