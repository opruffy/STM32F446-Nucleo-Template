/*
 * commutation.h
 *
 *  Created on: 13.06.2019
 *      Author: MT-LAB
 */

#ifndef COMMUTATION_H_
#define COMMUTATION_H_

void commutation_force();
void commutation_set(uint8_t counter);
void commutation_irq_gpio(uint16_t GPIO_Pin);
void commutation_irq_tim(void);

#endif /* COMMUTATION_H_ */
