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
	{	0, 	1,	1, 	0, 	0, 	0 },	// P2 - rising edge
	{	0, 	1, 	0, 	0, 	1, 	0 },	// P1 - falling edge
	{	0, 	0, 	0, 	1, 	1, 	0 },	// P3 - rising edge
	{	1, 	0, 	0, 	1, 	0, 	0 },	// P2 - falling edge
	{	1, 	0, 	0, 	0, 	0, 	1 },	// P1 - rising edge
	{	0, 	0, 	1, 	0, 	0, 	1 },	// P3 - falling edge
};


volatile uint8_t dutycycle = 100;
//volatile uint32_t period_rampe = PERIOD_START_VALUE;
//volatile uint32_t p_abtastung = PERIOD_START_VALUE*100;

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


uint8_t Calc_Period(uint32_t p)
{
	uint32_t apb2 = 90000000;
	uint32_t prescaler = 1;
	uint32_t period = 1;

	while(apb2 >= (prescaler * period * p))
	{
		if(prescaler < UINT16_MAX)
		{
			prescaler = prescaler << 1;
		}
		if(period < UINT16_MAX)
		{
			period = period << 1;
		}


	}

	return 0;

}

void kommutierung_counter(void)
{
	commutation_counter++;

	if(commutation_counter > 5)
	{
		commutation_counter = 0;
//		if(period_rampe > 200)
//		{
//			period_rampe -= 200;
//
////		if(period_rampe > 100)
////		{
////			period_rampe -= 100;
//
//			htim1.Init.Period = period_rampe;
//			HAL_TIM_Base_Init(&htim1);
//		}
	}
}

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

		if(BH1)
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
		    HAL_GPIO_WritePin(P1_EN_GPIO_Port, P1_EN_Pin, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);

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
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
		    HAL_GPIO_WritePin(P2_EN_GPIO_Port, P2_EN_Pin, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);

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
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
		    HAL_GPIO_WritePin(P3_EN_GPIO_Port, P3_EN_Pin, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);

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
