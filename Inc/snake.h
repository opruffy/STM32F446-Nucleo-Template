/*
 * snake.h
 *
 *  Created on: 19.06.2019
 *      Author: MT-LAB
 */

#ifndef SNAKE_H_
#define SNAKE_H_

typedef enum
{
    UP = 0,
    DOWN,
    LEFT,
    RIGHT,
} Direction;

void snake_start();

void snake_change_dir(Direction dir);

void snake_update();
void snake_get_matrix(uint8_t value[9][18]);

#endif /* SNAKE_H_ */
