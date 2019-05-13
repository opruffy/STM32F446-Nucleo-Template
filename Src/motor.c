/*
 * motor.c
 *
 *  Created on: 15.03.2018
 *      Author: peter
 */
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"
#include "stm32f4xx_hal_tim_ex.h"

#include "stm32f4xx_hal_def.h"

#include "motor.h"

#include "adc.h"
#include "tim.h"

static volatile uint8_t commutation_counter = 0;
const static uint8_t commutationTable[6][6] =
{
//		H1	L1	H2	L2	H3	L3
	{	0, 	1,	1, 	0, 	0, 	0 },	// P2 - rising edge
	{	0, 	1, 	0, 	0, 	1, 	0 },	// P1 - falling edge
	{	0, 	0, 	0, 	1, 	1, 	0 },	// P3 - rising edge
	{	1, 	0, 	0, 	1, 	0, 	0 },	// P2 - falling edge
	{	1, 	0, 	0, 	0, 	0, 	1 },	// P1 - rising edge
	{	0, 	0, 	1, 	0, 	0, 	1 },	// P3 - falling edge
};


void set_kommutierung(uint8_t counter)
{
	commutation_counter = counter;
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim1)
	{
		uint8_t BH1 = commutationTable[commutation_counter][0];
		uint8_t BL1 = commutationTable[commutation_counter][1];

		uint8_t BH2 = commutationTable[commutation_counter][2];
		uint8_t BL2 = commutationTable[commutation_counter][3];

		uint8_t BH3 = commutationTable[commutation_counter][4];
		uint8_t BL3 = commutationTable[commutation_counter][5];

		uint32_t dutycycle = 0;
		uint32_t adc_value = HAL_ADC_GetValue(&hadc1); //Get ADC Value
		dutycycle = adc_value * 100 / 4096;

		if(BH1)
		{
			__HAL_TIM_SET_DUTYCYCLE(&htim1, TIM_CHANNEL_1, dutycycle);
//			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
		    HAL_GPIO_WritePin(P1_EN_GPIO_Port, P1_EN_Pin, GPIO_PIN_SET);
		}
		else
		{
			__HAL_TIM_SET_DUTYCYCLE(&htim1, TIM_CHANNEL_1, 0);
//			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);

			if(BL1)
			{
			    HAL_GPIO_WritePin(P1_EN_GPIO_Port, P1_EN_Pin, GPIO_PIN_SET);
			}
			else
			{
			    HAL_GPIO_WritePin(P1_EN_GPIO_Port, P1_EN_Pin, GPIO_PIN_RESET);
			}
		}

		if(BH2)
		{
			__HAL_TIM_SET_DUTYCYCLE(&htim1, TIM_CHANNEL_2, dutycycle);
//			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
		    HAL_GPIO_WritePin(P2_EN_GPIO_Port, P2_EN_Pin, GPIO_PIN_SET);
		}
		else
		{
			__HAL_TIM_SET_DUTYCYCLE(&htim1, TIM_CHANNEL_2, 0);
//			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);

			if(BL2)
			{
			    HAL_GPIO_WritePin(P2_EN_GPIO_Port, P2_EN_Pin, GPIO_PIN_SET);
			}
			else
			{
			    HAL_GPIO_WritePin(P2_EN_GPIO_Port, P2_EN_Pin, GPIO_PIN_RESET);
			}
		}

		if(BH3)
		{
			__HAL_TIM_SET_DUTYCYCLE(&htim1, TIM_CHANNEL_3, dutycycle);
//			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
		    HAL_GPIO_WritePin(P3_EN_GPIO_Port, P3_EN_Pin, GPIO_PIN_SET);
		}
		else
		{
			__HAL_TIM_SET_DUTYCYCLE(&htim1, TIM_CHANNEL_3, 0);
//			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);

			if(BL3)
			{
			    HAL_GPIO_WritePin(P3_EN_GPIO_Port, P3_EN_Pin, GPIO_PIN_SET);
			}
			else
			{
			    HAL_GPIO_WritePin(P3_EN_GPIO_Port, P3_EN_Pin, GPIO_PIN_RESET);
			}
		}
	}
}
