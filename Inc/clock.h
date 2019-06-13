/*
 * clock.h
 *
 *  Created on: 13.06.2019
 *      Author: MT-LAB
 */

#ifndef CLOCK_H_
#define CLOCK_H_

void clock_inc(void);

void clock_setSec(uint8_t value);
void clock_setMin(uint8_t value);
void clock_setHour(uint8_t value);

uint8_t clock_getSec(void);
uint8_t clock_getMin(void);
uint8_t clock_getHour(void);

#endif /* CLOCK_H_ */
