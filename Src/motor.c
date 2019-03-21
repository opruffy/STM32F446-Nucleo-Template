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

#include "board_utils.h"
#include "tim.h"

#define LL_TIM_CHANNEL_CH1		TIM_CHANNEL_1
#define LL_TIM_CHANNEL_CH2		TIM_CHANNEL_2
#define LL_TIM_CHANNEL_CH3		TIM_CHANNEL_3

#define LL_TIM_CHANNEL_CH1N		TIM_CHANNEL_1
#define LL_TIM_CHANNEL_CH2N		TIM_CHANNEL_2
#define LL_TIM_CHANNEL_CH3N		TIM_CHANNEL_3

static volatile uint8_t commutation_counter = 0;
const static uint8_t commutationTable[6][6] =
{
//		H1	L1	H2	L2	H3	L3
	{	0, 	1,	1, 	0, 	0, 	0 },
	{	0, 	1, 	0, 	0, 	1, 	0 },
	{	0, 	0, 	0, 	1, 	1, 	0 },
	{	1, 	0, 	0, 	1, 	0, 	0 },
	{	1, 	0, 	0, 	0, 	0, 	1 },
	{	0, 	0, 	1, 	0, 	0, 	1 },
};


static volatile uint8_t schritt_counter = 0;
const static uint8_t schrittTable_right[4][4] =
{
//		S1a	S1b	S2a	S2b
	{	1, 	0,	1, 	0,	},
	{	1, 	0, 	0, 	1, 	},
	{	0, 	1, 	0, 	1, 	},
	{	0, 	1, 	1, 	0, 	},
};


volatile uint8_t dutycycle = 100;
volatile uint32_t motor = PERIOD_START_VALUE;

#ifdef USE_TIM_PWM
// PWM
TIM_OC_InitTypeDef sConfigOC_pwm =
{
	.OCMode = TIM_OCMODE_PWM1,
	.Pulse = 0xFFFF,
	.OCPolarity = TIM_OCPOLARITY_HIGH,
	.OCNPolarity = TIM_OCNPOLARITY_HIGH,
	.OCFastMode = TIM_OCFAST_DISABLE,
	.OCIdleState = TIM_OCIDLESTATE_RESET,
	.OCNIdleState = TIM_OCNIDLESTATE_RESET,
};

void LL_TIM_CC_EnableChannel(TIM_HandleTypeDef *htim, uint32_t Channel)
{
	HAL_TIM_OC_ConfigChannel(htim, &sConfigOC_pwm, Channel);
	__HAL_TIM_SET_DUTYCYCLE(htim, Channel, dutycycle);
	HAL_TIM_PWM_Start(htim, Channel);
}

void LL_TIM_CC_DisableChannel(TIM_HandleTypeDef *htim, uint32_t Channel)
{
	HAL_TIM_PWM_Stop(htim, Channel);
}

// activeLow
TIM_OC_InitTypeDef sConfigOC_activeLow =
{
	.OCMode = TIM_OCMODE_FORCED_ACTIVE,
	.Pulse = 0xFFFF,
	.OCPolarity = TIM_OCPOLARITY_LOW,
	.OCNPolarity = TIM_OCNPOLARITY_LOW,
	.OCFastMode = TIM_OCFAST_DISABLE,
	.OCIdleState = TIM_OCIDLESTATE_SET,
	.OCNIdleState = TIM_OCNIDLESTATE_SET,
};


void LL_TIM_CCN_EnableChannel(TIM_HandleTypeDef *htim, uint32_t Channel)
{
	HAL_TIM_OC_ConfigChannel(htim, &sConfigOC_activeLow, Channel);
	__HAL_TIM_SET_DUTYCYCLE(htim, Channel, dutycycle);
	HAL_TIM_OC_Start(htim, Channel);
}

void LL_TIM_CCN_DisableChannel(TIM_HandleTypeDef *htim, uint32_t Channel)
{
	HAL_TIM_OC_Stop(htim, Channel);
}
#endif

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim1)
	{
#ifdef USE_BLDC
		uint8_t BH1 = commutationTable[commutation_counter][0];
		uint8_t BL1 = commutationTable[commutation_counter][1];

		uint8_t BH2 = commutationTable[commutation_counter][2];
		uint8_t BL2 = commutationTable[commutation_counter][3];

		uint8_t BH3 = commutationTable[commutation_counter][4];
		uint8_t BL3 = commutationTable[commutation_counter][5];

		commutation_counter++;
		if(commutation_counter > 5)
			commutation_counter = 0;

		if(BH1)
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
		    HAL_GPIO_WritePin(P1_GPIO_Port, P1_Pin, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);

			if(BL1)
			{
			    HAL_GPIO_WritePin(P1_GPIO_Port, P1_Pin, GPIO_PIN_SET);
			}
			else
			{
			    HAL_GPIO_WritePin(P1_GPIO_Port, P1_Pin, GPIO_PIN_RESET);
			}
		}

		if(BH2)
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
		    HAL_GPIO_WritePin(P2_GPIO_Port, P2_Pin, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);

			if(BL2)
			{
			    HAL_GPIO_WritePin(P2_GPIO_Port, P2_Pin, GPIO_PIN_SET);
			}
			else
			{
			    HAL_GPIO_WritePin(P2_GPIO_Port, P2_Pin, GPIO_PIN_RESET);
			}
		}

		if(BH3)
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
		    HAL_GPIO_WritePin(P3_GPIO_Port, P3_Pin, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);

			if(BL3)
			{
			    HAL_GPIO_WritePin(P3_GPIO_Port, P3_Pin, GPIO_PIN_SET);
			}
			else
			{
			    HAL_GPIO_WritePin(P3_GPIO_Port, P3_Pin, GPIO_PIN_RESET);
			}
		}
#endif
	}
	else if(htim == &htim3)
	{
#ifdef USE_BLDC
		if(dutycycle < 100)
			dutycycle++;
		if(motor > 200)
		{
			motor-=200;
			htim1.Init.Period = motor;
			htim3.Init.Period = motor*10;
			HAL_TIM_Base_Init(&htim1);
			HAL_TIM_Base_Init(&htim3);
		}
#endif

		uint8_t BS1a = schrittTable_right[schritt_counter][0];
		uint8_t BS1b = schrittTable_right[schritt_counter][1];

		uint8_t BS2a = schrittTable_right[schritt_counter][2];
		uint8_t BS2b = schrittTable_right[schritt_counter][3];

		schritt_counter++;
		if(schritt_counter > 3)
			schritt_counter = 0;

		if(BS1a)
		{
			HAL_GPIO_WritePin(S1a_GPIO_Port, S1a_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(S1b_GPIO_Port, S1b_Pin, GPIO_PIN_RESET);
		}
		else
		{
			HAL_GPIO_WritePin(S1a_GPIO_Port, S1a_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(S1b_GPIO_Port, S1b_Pin, GPIO_PIN_SET);
		}

		if(BS2a)
		{
			HAL_GPIO_WritePin(S2a_GPIO_Port, S2a_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(S2b_GPIO_Port, S2b_Pin, GPIO_PIN_RESET);
		}
		else
		{
			HAL_GPIO_WritePin(S2a_GPIO_Port, S2a_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(S2b_GPIO_Port, S2b_Pin, GPIO_PIN_SET);
		}
	}
}
