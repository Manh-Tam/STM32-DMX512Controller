/*
 * flash.h
 *
 *  Created on: Mar 31, 2025
 *      Author: letam
 */

#ifndef INC_FLASH_H_
#define INC_FLASH_H_

#include <stdint.h>
#include <string.h>
#include "stm32f1xx.h"

#define DATA_PAGE_ADDR (0x08000000 + 63 * 1024) //page 63

void Flash_WriteWord(uint32_t pageAddr, uint32_t data);

void Flash_WriteDoubleWord(uint32_t pageAddr, uint64_t data);

uint32_t Flash_ReadWord(uint32_t pageAddr);

uint64_t Flash_ReadDoubleWord(uint32_t pageAddr);

void Flash_Erase(uint32_t pageAddr);

#endif /* INC_FLASH_H_ */
