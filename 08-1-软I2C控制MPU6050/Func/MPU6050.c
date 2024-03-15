#include "MPU6050.h"

#define MPU6050_ADDR 0xd0

void MPU6050_WriteReg(uint8_t RegAddr, uint8_t data)
{
	SI2C_Start();
	SI2C_SendByte(MPU6050_ADDR);	// 修改从机的寄存器指针
	SI2C_ReceiveAck();				// 从机应答
	SI2C_SendByte(RegAddr);			// 主机修改从机的寄存器指针
	SI2C_ReceiveAck();				// 从机应答
	SI2C_SendByte(data);			// 主机发送数据
	SI2C_ReceiveAck();				// 从机应答
	SI2C_Stop();					// 发送停止
}

uint8_t MPU6050_ReadReg(uint8_t RegAddr)
{
	SI2C_Start();
	SI2C_SendByte(MPU6050_ADDR);	// 只有读时序才能修改从机的寄存器指针
	SI2C_ReceiveAck();				// 从机应答
	SI2C_SendByte(RegAddr);			// 主机修改从机的寄存器指针
	SI2C_ReceiveAck();				// 从机应答（寄存器指针修改结束）
	
	SI2C_Start();						// 主机发起新的请求
	SI2C_SendByte(MPU6050_ADDR | 0x01);	// 主机发起读请求
	SI2C_ReceiveAck();					// 从机应答
	uint8_t data = SI2C_ReceiveByte();	// 主机接收数据
	SI2C_SendAck(1);					// 主机发送非应答
	SI2C_Stop();
	
	return data;
}

void MPU6050_Init(void)
{
	SI2C_Init();
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

