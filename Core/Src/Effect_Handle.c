/*
 * Effect_Handle.c
 *
 *  Created on: Aug 2, 2025
 *      Author: letam
 */

#include "Effect_Handle.h"

void GPIOA9_Init(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE();  // Bật clock cho GPIOA

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_9;      // Ch�?n chân PA2
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  // Output Push-Pull
    GPIO_InitStruct.Pull = GPIO_NOPULL;    // Không dùng Pull-up/Pull-down
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; // Tốc độ cao

    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);  // Khởi tạo chân PA2
}

void delay_us(uint16_t us)
{
	__HAL_TIM_SET_COUNTER(&htim2, 0);
	HAL_TIM_Base_Start(&htim2);
	while (__HAL_TIM_GET_COUNTER(&htim2) < us);
	HAL_TIM_Base_Stop(&htim2);
}

void BMAB_Init()
{
	GPIOA9_Init();
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, RESET);
	delay_us(100);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, SET);
	delay_us(1);
}
//
//void RunEffect(Effect_t effect)
//{
//	switch (effect.EffectItemNum)
//	{
//	case 0:
//	  Rainbow();
//	  break;
//	case 1:
//	  RainbowWipe();
//	  break;
//	case 2:
//	  Chase();
//	  break;
//	case 3:
//	  Breathing();
//	  break;
//	case 4:
//	  RainbowBreathing();
//	  break;
//	case 5:
//	  Custom();
//	  break;
//	default:
//	  RainbowWipe();
//	  break;
//	}
//}

const EffectEntry_t effectTable[] =
{
		{0, Rainbow},
		{1, RainbowWipe},
		{2, Chase},
		{3, Breathing},
		{4, RainbowBreathing},
		{5, Custom},
};

const EffectEntry_t* FindEffectHandler(uint8_t effectId)
{
	const EffectEntry_t* effectEntry = NULL;

	uint8_t effectTableSize = (sizeof(effectTable) / sizeof(effectTable[0]));

	for (uint8_t i = 0; i < effectTableSize; i++)
		if (effectId == effectTable[i].id)
		{
			effectEntry = &effectTable[i];
			return effectEntry;
		}
	return NULL;
}

void RunEffect(Effect_t effect)
{
	const EffectEntry_t* effectEntry = FindEffectHandler(effect.EffectItemNum);
	if (effectEntry)
		effectEntry->function();
}

void StoreEffectIntoFlash(uint32_t pageAddr)
{
	if (flash_flag)
	{
		Flash_Erase(pageAddr);
		HAL_FLASH_Unlock();
		for (uint8_t i = 0; i < num; i++)
		{
			Flash_WriteEffect(pageAddr + (i * 8), RxBuffer[i]);
		}
		HAL_FLASH_Lock();
		flash_flag = 0;
	}
}

void ReadEffectFromFlash(uint32_t pageAddr)
{
	num = Flash_ReadWord(pageAddr);
	for (uint8_t i = 0; i < num; i++)
	{
	  uint64_t data = Flash_ReadDoubleWord(pageAddr + (i * 8));
	  RxBuffer[i].EffectItemNum = (data >> 56) & 0xff;
	  RxBuffer[i].HourDuration = (data >> 48) & 0xff;
	  RxBuffer[i].MinuteDuration = (data >> 40) & 0xff;
	  RxBuffer[i].SecondDuration = (data >> 32) & 0xff;
	  RxBuffer[i].Color_R = (data >> 24) & 0xff;
	  RxBuffer[i].Color_G = (data >> 16) & 0xff;
	  RxBuffer[i].Color_B = (data >> 8) & 0xff;
	}
}

void UpdateEffect(Effect_t effect, uint32_t *previousTick)
{
	//calculate the duration to switch between effects
	uint32_t duration = effect.HourDuration * 60 * 60
					+ effect.MinuteDuration * 60
					+ effect.SecondDuration;

	//switch an effect if it reaches the calculated duration
	uint32_t currentTick = HAL_GetTick();
	if (currentTick - *previousTick > duration * 1000)
	{
	  *previousTick = HAL_GetTick();
	  currentEffect++;
	  if (currentEffect >= num)
		  currentEffect = 0;
	}
}
