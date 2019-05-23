/*
 * motor.h
 *
 *  Created on: 15.03.2018
 *      Author: peter
 */

#ifndef MOTOR_H_
#define MOTOR_H_

void HAL_TIM_PeriodElapsedCallback();

void set_kommutierung(uint8_t counter);

void set_active_led(uint8_t tmp);
uint8_t get_active_led();

#endif /* MOTOR_H_ */
