#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Serial.h"

extern uint8_t Str_receive[];

int main(void)
{
	OLED_Init();
	Serial_UserInit();
	
	OLED_ShowString(2,1,"HELLO");
	Serial_PrintString("Hello world!");
	
	while(1)
	{
		char* buff = Serial_GetString();
		if(buff != (uint8_t)0x00)	// 只有当返回值不是空指针的时候，数据才有效
		{	
			OLED_ShowString(1, 1, buff);
			Serial_PrintString(buff);			
		}
	}
}



