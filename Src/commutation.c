/*
 * commutation.c
 *
 *  Created on: 13.06.2019
 *      Author: MT-LAB
 */

#include <stdint.h>

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_conf.h"

#include "tim.h"
#include "terminal.h"

volatile uint32_t dutycycle = 100;
uint8_t commutation_old_state = 0;

static volatile uint8_t commutation_counter = 6;
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

void commutation_force()
{
	if(commutation_counter == 0)
		commutation_counter = 6;

	commutation_counter--;

	if(HAL_GPIO_ReadPin(SWITCH_GPIO_Port, SWITCH_Pin))
		dutycycle = 0;
	else
		dutycycle = 70;
}

float esum = 0;

float Kp = 0;
float Ki = 0;

int32_t regler(uint32_t speed)
{
	int32_t e = speed - 31250;
//	int32_t e = speed - 16250;
//	int32_t e = speed - 62500;
	uint32_t ruhelage = 70;
//	uint32_t ruhelage = 59;
//	uint32_t ruhelage = 30;

	esum = esum + e/100;
	if( esum > 100 )
		esum = 100;
	else if(esum < -100)
		esum = -100;

	Kp = 1;
	Ki = 1;

	int32_t y = Kp * e + Ki * esum + ruhelage;

	uint8_t pData[3] = {0};

	if(e < 0)
	{
		pData[0] = '-';
		pData[1] = (uint8_t)e * (-1);
	}
	else
	{
		pData[0] = '+';
		pData[1] = (uint8_t)e;
	}

	pData[2] = y;

	terminal_tx(pData, sizeof(pData));

	if( y > 85 )
		y = 85;
	else if( y < 0 )
		y = 0;

	return y;
}


void commutation_set(uint8_t counter)
{
	uint32_t speed = __HAL_TIM_GET_COUNTER(&htim10) * 240;
	__HAL_TIM_SET_COUNTER(&htim10, 0);

	commutation_counter = counter;

	if(HAL_GPIO_ReadPin(SWITCH_GPIO_Port, SWITCH_Pin))
		dutycycle = 0;
	else
		dutycycle = regler(speed);
}


void commutation_irq_gpio(uint16_t GPIO_Pin)
{
	switch(GPIO_Pin)
	{
		case P1_COMP_Pin: //External Interrupt on B1

			if(HAL_GPIO_ReadPin(P1_COMP_GPIO_Port, P1_COMP_Pin) != GPIO_PIN_RESET)
			{
				// P1 rising edge
				if(commutation_old_state == 5)
				{
					commutation_set(4);
					commutation_old_state = 4;
				}
			}
			else
			{
				// P1 falling edge
				if(commutation_old_state == 2)
				{
					commutation_set(1);
					commutation_old_state = 1;
				}
			}
			break;

		case P2_COMP_Pin:

			if(HAL_GPIO_ReadPin(P2_COMP_GPIO_Port, P2_COMP_Pin) != GPIO_PIN_RESET)
			{
				// P2 rising edge
				if(commutation_old_state == 1)
				{
					commutation_set(0);
					commutation_old_state = 0;
				}
			}
			else
			{
				// P2 falling edge
				if(commutation_old_state == 4)
				{
					commutation_set(3);
					commutation_old_state = 3;
				}
			}
			break;

		case P3_COMP_Pin:

			if(HAL_GPIO_ReadPin(P3_COMP_GPIO_Port, P3_COMP_Pin) != GPIO_PIN_RESET)
			{
				// P3 rising edge
				if(commutation_old_state == 3)
				{
					commutation_set(2);
					commutation_old_state = 2;
				}
			}
			else
			{
				// P3 falling edge
				if(commutation_old_state == 0)
				{
					commutation_set(5);
					commutation_old_state = 5;
				}
			}
			break;

		default:
			break;
	}
}


void commutation_irq_tim()
{
	uint8_t BH1 = commutationTable[commutation_counter][0];
	uint8_t BL1 = commutationTable[commutation_counter][1];

	uint8_t BH2 = commutationTable[commutation_counter][2];
	uint8_t BL2 = commutationTable[commutation_counter][3];

	uint8_t BH3 = commutationTable[commutation_counter][4];
	uint8_t BL3 = commutationTable[commutation_counter][5];

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
