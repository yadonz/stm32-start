#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "ADC.h"


int main(void)
{
	uint16_t ad_value;
	uint32_t cnt;
	OLED_Init();
	AD_Init();
	
	
	while(1)
	{
		ad_value = AD_GetValue();
		OLED_ShowNum(1, 1, ad_value*33/40950, 1);
		OLED_ShowChar(1, 2, '.');
		OLED_ShowNum(1, 3, ad_value*33/4095%10, 1);
		OLED_ShowChar(1, 4, 'V');
		for(cnt = 720000*3; cnt; cnt --);
	}
}
