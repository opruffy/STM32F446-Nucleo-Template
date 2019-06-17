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
uint8_t day = 17;
uint8_t month = 6;
uint8_t year = 19;


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

				if(day > 31)
				{
					month++;
					day = 1;

					if(month > 12)
					{
						year++;
						month = 1;
					}
				}
			}
		}

	}
}

void clock_setSec(uint8_t value)
{
	if(sec < 60)
		sec = value;
}

void clock_setMin(uint8_t value)
{
	if(min < 60)
		min = value;
}

void clock_setHour(uint8_t value)
{
	if(hour < 24)
		hour = value;
}

void clock_setDay(uint8_t value)
{
	if((day > 0) && (day <= 31))
		day = value;
}

void clock_setMonth(uint8_t value)
{
	if((month > 0) && (month <= 12))
		month = value;
}

void clock_setYear(uint8_t value)
{
	year = value;
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

uint8_t clock_getDay()
{
	return day;
}

uint8_t clock_getMonth()
{
	return month;
}

uint8_t clock_getYear()
{
	return year;
}
