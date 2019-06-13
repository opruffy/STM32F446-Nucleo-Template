/*
 * clock.c
 *
 *  Created on: 13.06.2019
 *      Author: MT-LAB
 */

#include <stdint.h>

uint8_t sec = 55;
uint8_t min = 58;
uint8_t hour = 0;
uint8_t day = 0;
uint8_t month = 0;
uint8_t year = 0;


void clock_inc()
{
	sec++;

	if(sec >= 60)
	{
		min++;
		sec = 0;

		if(min >= 60)
		{
			hour++;
			min = 0;

			if(hour >= 24)
			{
				day++;
				hour = 0;
			}
		}

	}
}

void clock_setSec(uint8_t value)
{
	sec = value;
}

void clock_setMin(uint8_t value)
{
	min = value;
}

void clock_setHour(uint8_t value)
{
	hour = value;
}

uint8_t clock_getSec()
{
	return sec;
}

uint8_t clock_getMin()
{
	return min;
}

uint8_t clock_getHour()
{
	return hour;
}
