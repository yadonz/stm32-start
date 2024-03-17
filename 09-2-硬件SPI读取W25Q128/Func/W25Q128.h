#ifndef __W25Q128_H
#define __W25Q128_H

#include "stm32f10x.h"                  // Device header
#include "SPI.h"


#define W25Q128_INSTRUCTION_WriteEnable 	0x06
#define W25Q128_INSTRUCTION_ReadStatusReg	0x05
#define W25Q128_INSTRUCTION_PageProgram		0x02
#define W25Q128_INSTRUCTION_SectorEarse		0x20
#define W25Q128_INSTRUCTION_JEDEC_id		0x9f
#define W25Q128_INSTRUCTION_ReadData		0x03

#define DUMMY_BYTE							0xff

void W25Q128_Init(void);
void W25Q128_GetID(uint8_t* mid, uint16_t* did);
void W25Q128_WriteEnable(void);
void W25Q128_WaitBusy(void);
void W25Q128_SectorErase(uint32_t addr);
void W25Q128_WriteData(uint32_t addr, uint8_t* sdata, uint16_t length);
void W25Q128_ReadData(uint32_t addr, uint8_t* rdata, uint16_t length);



#endif //__W25Q128_H
