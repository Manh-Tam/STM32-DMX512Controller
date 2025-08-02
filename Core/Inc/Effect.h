/*
 * Effect.h
 *
 *  Created on: Jul 9, 2025
 *      Author: letam
 */

#ifndef INC_EFFECT_H_
#define INC_EFFECT_H_

#include <stdint.h>
#include "stm32f1xx.h"
#include <math.h>
#include <stdlib.h>

#define NUM_OF_LIGHT  512

extern UART_HandleTypeDef huart1;



typedef struct
{
	uint8_t EffectItemNum;
	uint8_t HourDuration;
	uint8_t MinuteDuration;
	uint8_t SecondDuration;
	uint8_t Color_R;
	uint8_t Color_G;
	uint8_t Color_B;
	uint8_t reserved;
}Effect_t;

extern Effect_t RxBuffer[80];
extern uint8_t currentEffect;

void Rainbow();
void RainbowWipe();
void Chase();
void Breathing();
void RainbowBreathing();
void Custom();


#endif /* INC_EFFECT_H_ */
