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
#include "snake.h"
#include "symbol_table.h"
#include "terminal.h"

#define SCREEN_MAX_SIZE	108

static volatile uint32_t slot = 0;
uint32_t slot_counter = 0;

uint32_t symbol_counter = 0;
static volatile uint8_t led_counter = 0;


uint32_t symbol_start = 0;
uint32_t symbol_end = 0;
uint32_t symbol_breite = 0;
uint32_t symbol_max_count = 0;

uint8_t timeTable[] = {0, 0, 10, 0, 0, 10, 0, 2, 12, 0, 1, 11, 0, 1, 11, 1, 9, 12};
uint8_t pacmanTable[] = {0, 1, 2};

uint8_t matrix_show[9][18]={0};

// delay for movement of symbols
uint32_t delay = 0;

uint8_t type = 0;
uint8_t rot = 0;

void led_symbol_counter_inc()
{
	symbol_counter++;
	if(symbol_counter > symbol_max_count-1)
		symbol_counter = 0;
}

void led_resetValue()
{
	switch (type)
	{
		case 0:
			// TIME / DATE
			symbol_breite = sizeof(symbol_table[0][0]);
			symbol_max_count = sizeof(timeTable);

			symbol_start = 0;
			symbol_end = symbol_start + symbol_breite * symbol_max_count + symbol_max_count;

			symbol_end *= 10;

			if(rot == 1)
			{
				delay++;
				if(delay > 3000)
				{
					slot_counter++;
					if(slot_counter > 9)
					{
						slot_counter = 0;
						led_counter++;
						if(led_counter > symbol_breite-1)
						{
							led_counter = 0;
						}
					}
					delay = 0;
				}
			}
			else
			{
				led_counter =  0;
				symbol_counter = 0;
				slot_counter = 0;
				delay = 0;
			}
			break;
		case 1:
			// PACMAN
			symbol_breite = sizeof(symbol_pacman[0][0]);
			symbol_max_count = sizeof(pacmanTable)-1;

			symbol_counter = 2;

			delay++;
			if(delay > 4)
			{
				if(symbol_start == 0)
				{
					symbol_start = SCREEN_MAX_SIZE - symbol_max_count * symbol_breite - symbol_max_count;
					symbol_end = symbol_start + symbol_max_count * symbol_breite + symbol_max_count;
				}
				symbol_start--;
				symbol_end--;

				delay = 0;
			}

			led_counter =  0;
			break;

		case 2:
			// SNAKE
			symbol_breite = sizeof(matrix_show[0]);
			symbol_max_count = 1;

			symbol_start = 0;
			symbol_end = symbol_start + symbol_breite * symbol_max_count + symbol_max_count;

			delay++;
			if(delay > 30)
			{
				snake_update();
				snake_get_matrix(matrix_show);

				delay = 0;
			}

			led_counter = 0;

		default:
			break;
	}

	slot = 0;


//	symbol_counter = 0;
//	led_counter = 0;
}

void led_set_type()
{
	type++;
	if(type > 2)
		type = 0;

	if(type == 2)
		snake_start();

	delay = 0xFF-1;

	led_resetValue();
}

void led_set_rot()
{
	rot++;
	if(rot > 1)
		rot = 0;

	led_resetValue();
}

void led_speed()
{
	uint32_t speed = __HAL_TIM_GET_COMPARE(&htim5, TIM_CHANNEL_2);
	uint32_t timeslot = 0;

	if(type == 0)
		timeslot = (speed / 120);
	else
		timeslot = (speed / 12);

	__HAL_TIM_SET_AUTORELOAD(&htim8, timeslot);
	__HAL_TIM_SET_COUNTER(&htim8, timeslot-1);

	led_resetValue();
}

void led_set_timetable()
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
}

void led_irq()
{
	if((slot >= symbol_start) && (slot < symbol_end))
	{
		static uint8_t state = 1;

		if(state)
		{
			if(type == 0)
			{
				led_set_timetable();

				HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, symbol_table[timeTable[symbol_counter]][0][led_counter]);
				HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, symbol_table[timeTable[symbol_counter]][1][led_counter]);
				HAL_GPIO_WritePin(LED_3_GPIO_Port, LED_3_Pin, symbol_table[timeTable[symbol_counter]][2][led_counter]);
				HAL_GPIO_WritePin(LED_4_GPIO_Port, LED_4_Pin, symbol_table[timeTable[symbol_counter]][3][led_counter]);
				HAL_GPIO_WritePin(LED_5_GPIO_Port, LED_5_Pin, symbol_table[timeTable[symbol_counter]][4][led_counter]);
				HAL_GPIO_WritePin(LED_6_GPIO_Port, LED_6_Pin, symbol_table[timeTable[symbol_counter]][5][led_counter]);
				HAL_GPIO_WritePin(LED_7_GPIO_Port, LED_7_Pin, symbol_table[timeTable[symbol_counter]][6][led_counter]);
				HAL_GPIO_WritePin(LED_8_GPIO_Port, LED_8_Pin, symbol_table[timeTable[symbol_counter]][7][led_counter]);
				HAL_GPIO_WritePin(LED_9_GPIO_Port, LED_9_Pin, GPIO_PIN_SET);

				slot_counter++;
				if(slot_counter > 9)
				{
					slot_counter = 0;
					led_counter++;
					if(led_counter > symbol_breite-1)
					{
						led_counter = 0;
						state = 0;
						led_symbol_counter_inc();
					}
				}
			}
			else if(type == 1)
			{
				static uint8_t symbol_old=0;

				HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, symbol_pacman[pacmanTable[symbol_counter]][0][led_counter]);
				HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, symbol_pacman[pacmanTable[symbol_counter]][1][led_counter]);
				HAL_GPIO_WritePin(LED_3_GPIO_Port, LED_3_Pin, symbol_pacman[pacmanTable[symbol_counter]][2][led_counter]);
				HAL_GPIO_WritePin(LED_4_GPIO_Port, LED_4_Pin, symbol_pacman[pacmanTable[symbol_counter]][3][led_counter]);
				HAL_GPIO_WritePin(LED_5_GPIO_Port, LED_5_Pin, symbol_pacman[pacmanTable[symbol_counter]][4][led_counter]);
				HAL_GPIO_WritePin(LED_6_GPIO_Port, LED_6_Pin, symbol_pacman[pacmanTable[symbol_counter]][5][led_counter]);
				HAL_GPIO_WritePin(LED_7_GPIO_Port, LED_7_Pin, symbol_pacman[pacmanTable[symbol_counter]][6][led_counter]);
				HAL_GPIO_WritePin(LED_8_GPIO_Port, LED_8_Pin, symbol_pacman[pacmanTable[symbol_counter]][7][led_counter]);
				HAL_GPIO_WritePin(LED_9_GPIO_Port, LED_9_Pin, symbol_pacman[pacmanTable[symbol_counter]][8][led_counter]);

				led_counter++;
				if(led_counter > symbol_breite-1)
				{
					if(symbol_counter == 0)
					{
						symbol_old = 0;
						symbol_counter = 2;
					}
					else if(symbol_counter == 1)
					{
						symbol_old = 1;
						symbol_counter = 2;
					}
					else if(symbol_counter == 2)
					{
						if(symbol_old == 0)
							symbol_counter = 1;
						else if(symbol_old == 1)
							symbol_counter = 0;
					}

					led_counter = 0;
					state = 0;
				}
			}
			else if(type == 2)
			{
				HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, matrix_show[0][led_counter]);
				HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, matrix_show[1][led_counter]);
				HAL_GPIO_WritePin(LED_3_GPIO_Port, LED_3_Pin, matrix_show[2][led_counter]);
				HAL_GPIO_WritePin(LED_4_GPIO_Port, LED_4_Pin, matrix_show[3][led_counter]);
				HAL_GPIO_WritePin(LED_5_GPIO_Port, LED_5_Pin, matrix_show[4][led_counter]);
				HAL_GPIO_WritePin(LED_6_GPIO_Port, LED_6_Pin, matrix_show[5][led_counter]);
				HAL_GPIO_WritePin(LED_7_GPIO_Port, LED_7_Pin, matrix_show[6][led_counter]);
				HAL_GPIO_WritePin(LED_8_GPIO_Port, LED_8_Pin, matrix_show[7][led_counter]);
				HAL_GPIO_WritePin(LED_9_GPIO_Port, LED_9_Pin, matrix_show[8][led_counter]);

				led_counter++;
				if(led_counter > symbol_breite-1)
				{
					led_counter = 0;
					state = 0;
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

			if(state == 0)
			{
				slot_counter++;

				if(slot_counter > 3)
				{
					slot_counter=0;
					state = 1;
				}
			}
			else
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



