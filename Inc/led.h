/*
 * led.h
 *
 *  Created on: 13.06.2019
 *      Author: MT-LAB
 */

#ifndef LED_H_
#define LED_H_

void led_resetValue(void);
void led_speed(void);
void led_speed_irq(uint16_t GPIO_Pin);
void led_irq(void);

#endif /* LED_H_ */
