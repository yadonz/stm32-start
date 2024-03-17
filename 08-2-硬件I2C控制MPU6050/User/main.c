#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MPU6050.h"

#define DELAY_TIM TIM2

int main(void)
{
	MPU6050_DataType MPU6050_DataStructure;
	
	OLED_Init();
	MPU6050_Init();
	
	OLED_ShowString(1, 1, "acc     gyro");
	
	
	while (1)
	{
		MPU6050_GetData(&MPU6050_DataStructure);
		OLED_ShowSignedNum(2, 1, MPU6050_DataStructure.ax, 5);
		OLED_ShowSignedNum(3, 1, MPU6050_DataStructure.ay, 5);
		OLED_ShowSignedNum(4, 1, MPU6050_DataStructure.az, 5);
		OLED_ShowSignedNum(2, 9, MPU6050_DataStructure.gx, 5);
		OLED_ShowSignedNum(3, 9, MPU6050_DataStructure.gy, 5);
		OLED_ShowSignedNum(4, 9, MPU6050_DataStructure.gz, 5);
		Delay_ms(100);
	}
}
