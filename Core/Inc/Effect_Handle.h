/*
 * Effect_Handle.h
 *
 *  Created on: Aug 2, 2025
 *      Author: letam
 */

#ifndef INC_EFFECT_HANDLE_H_
#define INC_EFFECT_HANDLE_H_
#include "stm32f1xx_hal.h"
#include "Effect.h"
#include "flash.h"

typedef void (*EffectHandler_t)();

typedef struct
{
	uint8_t id;
	EffectHandler_t function;
}EffectEntry_t;

extern uint8_t flash_flag;
extern uint8_t num;
extern TIM_HandleTypeDef htim2;

void GPIOA2_Init(void);
void delay_us(uint16_t us);
void BMAB_Init();
void RunEffect(Effect_t effect);
void StoreEffectIntoFlash(uint32_t pageAddr);
void ReadEffectFromFlash();
void UpdateEffect(Effect_t effect, uint32_t *previousTick);



#endif /* INC_EFFECT_HANDLE_H_ */
