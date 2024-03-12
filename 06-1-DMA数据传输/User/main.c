#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "ADC.h"
#include "DMA.h"

uint8_t Data[4] = {0x11, 0x22, 0x33, 0x44};
uint8_t Datad[4] = {0};

int main(void)
{
	uint32_t cnt;
	uint32_t i;

	OLED_Init();
	
	// DMA 转运前的数据
	for(i = 0; i < 4; i ++)
	{
		OLED_ShowHexNum(1, i*3 + 1, Data[i], 2);
	}
	for(i = 0; i < 4; i ++)
	{
		OLED_ShowHexNum(2, i*3 + 1, Datad[i], 2);
	}
	// 配置并开始 DMA 转运
	DMA_UserInit((uint32_t)Data, (uint32_t)Datad, 4);
	// DMA 转运后的数据
	for(i = 0; i < 4; i ++)
	{
		OLED_ShowHexNum(3, i*3 + 1, Data[i], 2);
	}
	for(i = 0; i < 4; i ++)
	{
		OLED_ShowHexNum(4, i*3 + 1, Datad[i], 2);
	}
	
	while(1)
	{

		for(cnt = 720000*3; cnt; cnt --);
	}
}
