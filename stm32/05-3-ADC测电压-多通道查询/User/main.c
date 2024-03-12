#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "ADC.h"


int main(void)
{
	uint16_t ad_value;
	uint16_t resistor;
	uint32_t cnt;
	OLED_Init();
	AD_Init();
	
	
	while(1)
	{
		ad_value = AD_GetValue(ADC_Channel_8);		// 从通道 8 采集电压
		OLED_ShowNum(1, 1, ad_value*33/40950, 1);
		OLED_ShowChar(1, 2, '.');
		OLED_ShowNum(1, 3, ad_value*33/4095%10, 1);
		OLED_ShowChar(1, 4, 'V');
		
		ad_value = AD_GetValue(ADC_Channel_9);		// 从通道 9 采集电压
		resistor = (5*ad_value*100)/(4096 - ad_value);
		OLED_ShowNum(2, 1, resistor/100, 2);
		OLED_ShowChar(2, 3, '.');
		OLED_ShowNum(2, 4, resistor%100, 2);
		OLED_ShowChar(2, 5, 'K');
		for(cnt = 720000*3; cnt; cnt --);
	}
}
