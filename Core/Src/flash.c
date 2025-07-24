/*
 * flash.c
 *
 *  Created on: Mar 31, 2025
 *      Author: letam
 */
#include "flash.h"

void Flash_Erase(uint32_t pageAddr)
{
	HAL_FLASH_Unlock();
	FLASH_EraseInitTypeDef EraseInit =
	{
			.Banks = FLASH_BANK_1,
			.NbPages = 1,
			.PageAddress = pageAddr,
			.TypeErase = FLASH_TYPEERASE_PAGES,
	};
	uint32_t PageError = 0;
	HAL_FLASHEx_Erase(&EraseInit, &PageError);
	HAL_FLASH_Lock();
}

void Flash_WriteWord(uint32_t pageAddr, uint32_t data)
{
	Flash_Erase(pageAddr);
	HAL_FLASH_Unlock();
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, pageAddr, data);
	HAL_FLASH_Lock();
}

void Flash_WriteDoubleWord(uint32_t pageAddr, uint64_t data)
{
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, pageAddr, data);
}

uint32_t Flash_ReadWord(uint32_t pageAddr)
{
	return *(volatile uint32_t*)pageAddr;
}

uint64_t Flash_ReadDoubleWord(uint32_t pageAddr)
{
	return *(volatile uint64_t*)pageAddr;
}
