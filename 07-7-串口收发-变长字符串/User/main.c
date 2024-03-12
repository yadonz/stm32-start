#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Serial.h"

extern uint8_t Str_receive[];

int main(void)
{
	OLED_Init();
	Serial_UserInit();
	
	while(1)
	{
		uint8_t* buff = Serial_Get8bytes();
		if(buff != (uint8_t)0x00)	// 只有当返回值不是空指针的时候，数据才有效
		{			
			for(int i = 0; i < 8; i ++)
			{
				OLED_ShowHexNum(1, 1 + i*2, buff[i], 2);
				Serial_PrintChar(buff[i]);
			}
		}
		for(uint32_t i = 720000; i; i --);
	}
}



