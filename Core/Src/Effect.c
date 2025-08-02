/*
 * Effect.c
 *
 *  Created on: Jul 10, 2025
 *      Author: letam
 */
#include "Effect.h"



//static void HSV_to_GRB(uint8_t hue, uint8_t *g, uint8_t *r, uint8_t *b) {
//    uint8_t region = hue / 43;
//    uint8_t remainder = (hue - (region * 43)) * 6;
//
//    uint8_t p = 0;
//    uint8_t q = (255 * (255 - remainder)) >> 8;
//    uint8_t t = (255 * remainder) >> 8;
//
//    switch (region) {
//        case 0: *r = 255; *g = t; *b = p; break;
//        case 1: *r = q; *g = 255; *b = p; break;
//        case 2: *r = p; *g = 255; *b = t; break;
//        case 3: *r = p; *g = q; *b = 255; break;
//        case 4: *r = t; *g = p; *b = 255; break;
//        default: *r = 255; *g = p; *b = q; break;
//    }
//}

void HSV_to_RGB(uint16_t hue, uint8_t *r, uint8_t *g, uint8_t *b)
{
	uint16_t increase = hue % 60 * 4;
	uint16_t decrease = 255 - increase;
	if (hue <= 59)
	{
		*r = 255;   *g = increase;   *b = 0;
	}
	else if (hue <= 119)
	{
		*r = decrease;   *g = 255;   *b = 0;
	}
	else if (hue <= 179)
	{
		*r = 0;     *g = 255;   *b = increase;
	}
	else if (hue <= 239)
	{
		*r = 0;     *g = decrease;   *b = 255;
	}
	else if (hue <= 299)
	{
		*r = increase;   *g = 0;     *b = 255;
	}
	else if (hue <= 359)
	{
		*r = 255;   *g = 0;     *b = decrease;
	}
}

void Rainbow()
{
	HAL_UART_Init(&huart1);
	static uint16_t base_hue = 0;
	static uint8_t data[3];
	for (int i = 0; i < 512; i += 3)
	{
		//uint8_t hue = base_hue + (i / 3) * 5; // dịch pha giữa các LED
		uint16_t hue = (base_hue * 3) % 360 ; // dịch pha giữa các LED
		//uint8_t r, g, b;
		HSV_to_RGB(hue, &data[0], &data[1], &data[2]);
		// GRB format: G at [0], R at [1], B at [2]

		HAL_UART_Transmit(&huart1, data, 3, 1000);
	}
	base_hue = (base_hue + 2) % 360;
	HAL_UART_DeInit(&huart1);
}

void RainbowWipe()
{
	HAL_UART_Init(&huart1);
	uint8_t temp = 0;
	HAL_UART_Transmit(&huart1, &temp, 1, 100);

	static uint16_t current_pos = 0;
	static uint16_t hue = 0;

	uint8_t data[] = {0, 255, 0};
	for (uint16_t i = 0; i < NUM_OF_LIGHT; i += 3)
	{
		if (i <= current_pos)
		{
			HSV_to_RGB(hue, &data[0], &data[1], &data[2]);
			HAL_UART_Transmit(&huart1, data, sizeof(data), 100);
		}

		else
		{
			uint8_t temp = 0;
			HAL_UART_Transmit(&huart1, &temp, 1, 100);
			HAL_UART_Transmit(&huart1, &temp, 1, 100);
			HAL_UART_Transmit(&huart1, &temp, 1, 100);
		}
	}
	current_pos = (current_pos + 3) % NUM_OF_LIGHT;
	hue = (hue + 1) % 360;
	HAL_UART_DeInit(&huart1);
}



void Chase()
{
	HAL_UART_Init(&huart1);
	uint8_t temp = 0;
	HAL_UART_Transmit(&huart1, &temp, 1, 100);

	static uint16_t current_pos = 0;

	uint8_t data[] = {0, 0, 255};
	for (int i = 0; i < NUM_OF_LIGHT; i += 3)
	{
		if (i == current_pos)
			HAL_UART_Transmit(&huart1, data, sizeof(data), 100);
		else
		{
			HAL_UART_Transmit(&huart1, &temp, 1, 100);
			HAL_UART_Transmit(&huart1, &temp, 1, 100);
			HAL_UART_Transmit(&huart1, &temp, 1, 100);
		}
	}
	HAL_UART_DeInit(&huart1);
	current_pos = (current_pos + 3) % NUM_OF_LIGHT;
}

void Breathing()
{
	HAL_UART_Init(&huart1);
	uint8_t data[3] = {255, 0, 0};
	static uint8_t up_down = 1;
	static uint8_t breathing = 0;
	for (uint16_t j = 0; j < NUM_OF_LIGHT; j++)
	{
		if (up_down == 1)
			breathing += 5;
		if (breathing >= 255)
			up_down = 0;
		if (up_down == 0)
			breathing -= 5;
		if (breathing <= 0)
			up_down = 1;
		data[0] = breathing;
		HAL_UART_Transmit(&huart1, data, sizeof(data), 100);
	}
	HAL_UART_DeInit(&huart1);
}

void RainbowBreathing()
{
	HAL_UART_Init(&huart1);
	uint8_t data[3] = {0, 0, 0};
	static uint16_t level = 0;
	static uint16_t base_hue = 0;


	HSV_to_RGB(base_hue, &data[0], &data[1], &data[2]);
	float brightness = fabs(cosf(level * 2  * 3.14 / 180.0f));
	data[0] = (data[0] * brightness);
	data[1] = (data[1] * brightness);
	data[2] = (data[2] * brightness);

	for (uint16_t j = 0; j < NUM_OF_LIGHT; j++)
	{
		HAL_UART_Transmit(&huart1, data, sizeof(data), 100);
	}
	HAL_UART_DeInit(&huart1);
	if (level == 0)
		base_hue = (base_hue + 30) % 360;
	level = (level + 2) % 360;

}

void Custom()
{
	HAL_UART_Init(&huart1);
	uint8_t temp = 0;
	HAL_UART_Transmit(&huart1, &temp, 1, 100);
	uint8_t data[3];
	data[0] = RxBuffer[currentEffect].Color_R;
	data[1] = RxBuffer[currentEffect].Color_G;
	data[2] = RxBuffer[currentEffect].Color_B;
	for (int i = 0; i < NUM_OF_LIGHT; i += 3)
	{
		HAL_UART_Transmit(&huart1, data, sizeof(data), 100);
	}
	HAL_UART_DeInit(&huart1);
}

