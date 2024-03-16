#include "MPU6050.h"

#define MPU6050_ADDR 0xd0
#define I2C_GPIO_RCC		RCC_APB2Periph_GPIOB
#define I2C_GPIO_PORT 		GPIOB
#define I2C_GPIO_SCL_PIN	GPIO_Pin_10
#define I2C_GPIO_SDA_PIN	GPIO_Pin_11


void MPU6050_WriteReg(uint8_t RegAddr, uint8_t data)
{
	I2C_GenerateSTART(I2C2, ENABLE);											// STM32 I2C 默认位从机模式，发送一个起始标志则进入主机模式
	while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS);		// 阻塞，直至进入主机模式
	
	I2C_Send7bitAddress(I2C2, MPU6050_ADDR, I2C_Direction_Transmitter);			// 发送从机地址（发送模式）
	while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS);// 阻塞，直至转变为发送模式
	
	I2C_SendData(I2C2, RegAddr);												// 发送寄存器地址
	while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING) != SUCCESS);	// 阻塞，直至移位寄存器空，数据寄存器非空
	
	I2C_SendData(I2C2, data);													// 发送数据
	while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS);	// 请求设置停止位
	
	I2C_GenerateSTOP(I2C2, ENABLE);												// 产生停止位
}

uint8_t MPU6050_ReadReg(uint8_t RegAddr)
{
	I2C_GenerateSTART(I2C2, ENABLE);											// STM32 I2C 默认位从机模式，发送一个起始标志则进入主机模式
	while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS);		// 阻塞，直至进入主机模式
	
	I2C_Send7bitAddress(I2C2, MPU6050_ADDR, I2C_Direction_Transmitter);			// 发送从机地址（发送模式）
	while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS);// 阻塞，直至转变为发送模式
	
	I2C_SendData(I2C2, RegAddr);												// 发送寄存器地址
	while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS);	// 阻塞，直至移位寄存器空，数据寄存器非空（transmitting 和 transmitted 都可以，建议 transmitted）
	
	I2C_GenerateSTART(I2C2, ENABLE);
	while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS);		// 阻塞，直至进入主机模式
	
	I2C_Send7bitAddress(I2C2, MPU6050_ADDR, I2C_Direction_Receiver);			// 发送从机地址（接收模式）
	while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) != SUCCESS);// 阻塞，直至转变为接收模式
	// 这里的操作必须要在最后一个字节发送之前执行（硬件会自动在最后一个字节发送之前缓存该操作，并在最后一个字节发送完成之后执行）
	I2C_AcknowledgeConfig(I2C2, DISABLE);										// 在最后一个字节发送之前，Ack 应答位关闭，并产生停止位
	I2C_GenerateSTOP(I2C2, ENABLE);												// 产生停止位
	
	while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED) != SUCCESS);		// 阻塞，直至数据寄存器非空
	uint8_t data = I2C_ReceiveData(I2C2);										// 接收数据
	
	// 读取完最后一个字节之后，将 Ack 置为 1（注意默认开启应答）
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	
	return data;
}

void MPU6050_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef I2C_InitStructure;
	
	RCC_APB2PeriphClockCmd(I2C_GPIO_RCC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Pin = I2C_GPIO_SCL_PIN | I2C_GPIO_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(I2C_GPIO_PORT, &GPIO_InitStructure);
	
	I2C_StructInit(&I2C_InitStructure);
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;			// 打开应答开关00
	I2C_InitStructure.I2C_ClockSpeed = 50000;			// 通信速度（时钟频率）
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2; 	// 时钟占空比(16:9, 2:1)
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;	
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;
	I2C_Init(I2C2, &I2C_InitStructure);
	
	I2C_Cmd(I2C2, ENABLE);
	
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);
	MPU6050_WriteReg(MPU6050_CONFIG, 0x06);
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);
}

void MPU6050_GetData(MPU6050_DataType* MPU6050_DataStructure)
{
	uint8_t DataH, DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
	MPU6050_DataStructure->ax = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
	MPU6050_DataStructure->ay = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
	MPU6050_DataStructure->az = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
	MPU6050_DataStructure->gx = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
	MPU6050_DataStructure->gy = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
	MPU6050_DataStructure->gz = (DataH << 8) | DataL;
}

uint8_t MPU6050_GetMPUId(void)
{
	return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}

