/*
 * led.c
 *
 *  Created on: 13.06.2019
 *      Author: MT-LAB
 */

#include <stdint.h>

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_conf.h"

#include "tim.h"

#include "clock.h"
#include "symbol_table.h"
#include "terminal.h"

static volatile uint32_t slot = 0;
uint32_t time_counter = 0;
static volatile uint8_t led_counter = 0;

uint8_t timeTable[TIMECOUNT] = {0, 0, 10, 0, 0, 10, 0, 2, 12, 0, 1, 11, 0, 1, 11, 1, 9, 12};


void led_resetValue()
{
	led_counter++;
	if(led_counter > ZAHLBREITE-1)
	{
		led_counter = 0;
		time_counter++;
		if(time_counter > TIMECOUNT-1)
		{
			time_counter = 0;
		}
	}

	slot = 0;
//	time_counter = 0;
//	led_counter = 0;
}

void led_speed()
{
//	uint32_t speed = __HAL_TIM_GET_COUNTER(&htim7);
//	__HAL_TIM_SET_COUNTER(&htim7, 0);

	uint32_t speed = __HAL_TIM_GET_COMPARE(&htim5, TIM_CHANNEL_2);

	uint32_t timeslot = (speed / 12); // *10 -> Einheitenumwandlung (10us->us); 5/220 -> Verhältnis Ledbreite zu Umfang
	__HAL_TIM_SET_AUTORELOAD(&htim8, timeslot);
	__HAL_TIM_SET_COUNTER(&htim8, 0);

	led_resetValue();

//	uint8_t pData[11] = {0};
//
//	pData[9] = speed % 10 + '0';
//	pData[8] = (speed / 10) % 10 + '0';
//	pData[7] = (speed / 100) % 10 + '0';
//	pData[6] = (speed / 1000) % 10 + '0';
//	pData[5] = (speed / 10000) % 10 + '0';
//	pData[4] = (speed / 100000) % 10 + '0';
//	pData[3] = (speed / 1000000) % 10 + '0';
//	pData[2] = (speed / 10000000) % 10 + '0';
//	pData[1] = (speed / 100000000) % 10 + '0';
//	pData[0] = (speed / 1000000000) % 10 + '0';
//	pData[10] = '\n';
//
//	terminal_tx(pData, sizeof(pData));
}

void led_speed_irq(uint16_t GPIO_Pin)
{
//	if(GPIO_Pin == RPM_Pin)
//	{
//		if(HAL_GPIO_ReadPin(RPM_GPIO_Port, RPM_Pin) != GPIO_PIN_RESET)
//		{
//			HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
//
//			//led_speed();
//		}
//	}
}

void led_irq()
{
	if(slot < (ZAHLBREITE*TIMECOUNT)+TIMECOUNT)
	{
		static uint8_t state = 1;

		if(state)
		{
			uint8_t sec = clock_getSec();
			uint8_t min = clock_getMin();
			uint8_t hour = clock_getHour();

			timeTable[7] = sec % 10;
			timeTable[6] = sec / 10;

			timeTable[4] = min % 10;
			timeTable[3] = min / 10;

			timeTable[1] = hour % 10;
			timeTable[0] = hour / 10;

			uint8_t day = clock_getDay();
			uint8_t month = clock_getMonth();
			uint8_t year = clock_getYear();

			timeTable[10] = day % 10;
			timeTable[9] = day / 10;

			timeTable[13] = month % 10;
			timeTable[12] = month / 10;

			timeTable[16] = year % 10;
			timeTable[15] = year / 10;

			HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, symbol_table[timeTable[time_counter]][0][led_counter]);
			HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, symbol_table[timeTable[time_counter]][1][led_counter]);
			HAL_GPIO_WritePin(LED_3_GPIO_Port, LED_3_Pin, symbol_table[timeTable[time_counter]][2][led_counter]);
			HAL_GPIO_WritePin(LED_4_GPIO_Port, LED_4_Pin, symbol_table[timeTable[time_counter]][3][led_counter]);
			HAL_GPIO_WritePin(LED_5_GPIO_Port, LED_5_Pin, symbol_table[timeTable[time_counter]][4][led_counter]);
			HAL_GPIO_WritePin(LED_6_GPIO_Port, LED_6_Pin, symbol_table[timeTable[time_counter]][5][led_counter]);
			HAL_GPIO_WritePin(LED_7_GPIO_Port, LED_7_Pin, symbol_table[timeTable[time_counter]][6][led_counter]);
			HAL_GPIO_WritePin(LED_8_GPIO_Port, LED_8_Pin, symbol_table[timeTable[time_counter]][7][led_counter]);
			HAL_GPIO_WritePin(LED_9_GPIO_Port, LED_9_Pin, GPIO_PIN_SET);

			led_counter++;
			if(led_counter > ZAHLBREITE-1)
			{
				led_counter = 0;
				state = 0;
				time_counter++;
				if(time_counter > TIMECOUNT-1)
				{
					time_counter = 0;
				}
			}
		}
		else
		{
			HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_3_GPIO_Port, LED_3_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_4_GPIO_Port, LED_4_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_5_GPIO_Port, LED_5_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_6_GPIO_Port, LED_6_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_7_GPIO_Port, LED_7_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_8_GPIO_Port, LED_8_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_9_GPIO_Port, LED_9_Pin, GPIO_PIN_RESET);

			state = 1;
		}
	}
	else
	{
		HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED_3_GPIO_Port, LED_3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED_4_GPIO_Port, LED_4_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED_5_GPIO_Port, LED_5_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED_6_GPIO_Port, LED_6_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED_7_GPIO_Port, LED_7_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED_8_GPIO_Port, LED_8_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED_9_GPIO_Port, LED_9_Pin, GPIO_PIN_RESET);
	}
	slot++;
}



