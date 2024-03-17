#include "W25Q128.h"

void W25Q128_Init(void)
{
	SPI_UserInit();
}

void W25Q128_GetID(uint8_t* mid, uint16_t* did)
{
	SPI_Start();
	
	SPI_ByteExchange(W25Q128_INSTRUCTION_JEDEC_id);
	*mid = SPI_ByteExchange(DUMMY_BYTE);
	*did = SPI_ByteExchange(DUMMY_BYTE) << 8;
	*did |= SPI_ByteExchange(DUMMY_BYTE);
	
	SPI_Stop();
}

void W25Q128_WriteEnable(void)
{// 每次写入操作前，都要写使能
	SPI_Start();
	
	SPI_ByteExchange(W25Q128_INSTRUCTION_WriteEnable);
	
	SPI_Stop();
}

void W25Q128_WaitBusy(void)
{
	uint16_t TimeOut;
	SPI_Start();
	while (SPI_ByteExchange(W25Q128_INSTRUCTION_ReadStatusReg)& 0x01)	// 如果存储器忙线中，则等待
		{// 超时则触发终止操作
		TimeOut ++;
		if (TimeOut >= 1000)
		{
			break;	// 这里应该添加超时后的异常处理
		}
	}
	SPI_Stop();
}

void W25Q128_SectorErase(uint32_t addr)
{
	W25Q128_WriteEnable();	// 写和擦除前一定要打开 写允许
	
	SPI_Start();
	
	SPI_ByteExchange(W25Q128_INSTRUCTION_SectorEarse);
	SPI_ByteExchange(addr >> 16);						// 写入地址 16 - 23 bits
	SPI_ByteExchange(addr >> 8);						// 写入地址 8 - 15 bits
	SPI_ByteExchange(addr);							// 写入地址 0 - 7 bits
	
	SPI_Stop();
	W25Q128_WaitBusy();	// 等待分为事前等待和事后等待，事前等待效率更高，但是读取操作也需要事前等待，而事后等待效率低，但是只需要写入和擦除操作等待即可
}

void W25Q128_WriteData(uint32_t addr, uint8_t* sdata, uint16_t length)
{
	W25Q128_WriteEnable();	// 写和擦除前一定要打开 写允许
	SPI_Start();
	
	SPI_ByteExchange(W25Q128_INSTRUCTION_PageProgram); // 页编程指令
	SPI_ByteExchange(addr >> 16);						// 写入地址 16 - 23 bits
	SPI_ByteExchange(addr >> 8);						// 写入地址 8 - 15 bits
	SPI_ByteExchange(addr);							// 写入地址 0 - 7 bits
	
	for (int i = 0; i < length; i ++)
	{
		SPI_ByteExchange(sdata[i]);
	}
	
	SPI_Stop();
	W25Q128_WaitBusy(); // 等待分为事前等待和事后等待，事前等待效率更高，但是读取操作也需要事前等待，而事后等待效率低，但是只需要写入和擦除操作等待即可
}

void W25Q128_ReadData(uint32_t addr, uint8_t* rdata, uint16_t length)
{
	SPI_Start();
	
	SPI_ByteExchange(W25Q128_INSTRUCTION_ReadData); 	// 页读取指令
	SPI_ByteExchange(addr >> 16);						// 写入地址 16 - 23 bits
	SPI_ByteExchange(addr >> 8);						// 写入地址 8 - 15 bits
	SPI_ByteExchange(addr);							// 写入地址 0 - 7 bits
	
	for (int i = 0; i < length; i ++)
	{
		rdata[i] = SPI_ByteExchange(DUMMY_BYTE);
	}
	
	SPI_Stop();
}

