#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "W25Q128.h"


int main(void)
{	
	uint8_t mid;
	uint16_t did;
	uint32_t addr = 0xfff000;
	uint8_t sdata[4] = {0x1a, 0x2b, 0x3c, 0x4d};
	uint8_t rdata[4] = {0};
	uint16_t length = 4;
	
	OLED_Init();
	W25Q128_Init();
	
	W25Q128_GetID(&mid, &did);
	OLED_ShowHexNum(1, 1, mid, 2);
	OLED_ShowHexNum(1, 8, did, 4);
	
	W25Q128_ReadData(addr, rdata, length);
	OLED_ShowHexNum(2, 1, rdata[0], 2);
	OLED_ShowHexNum(2, 4, rdata[1], 2);
	OLED_ShowHexNum(2, 7, rdata[2], 2);
	OLED_ShowHexNum(2, 10, rdata[3], 2);
	
	W25Q128_WriteData(addr, sdata, length);
	W25Q128_ReadData(addr, rdata, length);
	OLED_ShowHexNum(3, 1, rdata[0], 2);
	OLED_ShowHexNum(3, 4, rdata[1], 2);
	OLED_ShowHexNum(3, 7, rdata[2], 2);
	OLED_ShowHexNum(3, 10, rdata[3], 2);
	
	W25Q128_SectorErase(addr);
	W25Q128_ReadData(addr, rdata, length);
	OLED_ShowHexNum(4, 1, rdata[0], 2);
	OLED_ShowHexNum(4, 4, rdata[1], 2);
	OLED_ShowHexNum(4, 7, rdata[2], 2);
	OLED_ShowHexNum(4, 10, rdata[3], 2);
	
	
	while (1)
	{
	}
}
