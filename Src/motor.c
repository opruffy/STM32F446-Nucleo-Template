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

#include "clock.h"
#include "commutation.h"
#include "motor.h"
#include "led.h"
#include "symbol_table.h"

#include "adc.h"
#include "tim.h"
#include "usart.h"


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim1)
	{
		commutation_irq_tim();
	}
	else if(htim == &htim6)
	{
		;
	}
	else if(htim == &htim8)
	{
		led_irq();
	}
	else if(htim == &htim9)
	{
		static uint32_t counter = 0;

		counter ++;
		commutation_force();

		if(counter == 20)
		{
			HAL_TIM_Base_Stop_IT(&htim1);
			HAL_GPIO_WritePin(GPIOB, P3_EN_Pin|P2_EN_Pin|P1_EN_Pin, GPIO_PIN_RESET);
		}
		if(counter > 20)
		{
			HAL_TIM_Base_Stop_IT(&htim9);

			HAL_TIM_Base_Start_IT(&htim8);
			HAL_TIM_Base_Start_IT(&htim1);

			HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
		}
	}
	else if(htim == &htim11)
	{
		clock_inc();
	}
}
