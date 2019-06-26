/*
 * snake.c
 *
 *  Created on: 19.06.2019
 *      Author: MT-LAB
 */

#include <stdint.h>

uint8_t matrix[9][18]={0};

uint32_t body_length = 1;

typedef enum
{
    up = 0,
    down,
    left,
    right,
} Direction;

struct coord
{
    int8_t x;
    int8_t y;
    Direction direction;
};

struct coord head;

void snake_start()
{
	head.x = 0;
	head.y = 0;
	head.direction = down;
}

void snake_change_dir(Direction dir)
{
	head.direction = dir;
}

void snake_move()
{
	if(head.direction == up)
	{
		head.x--;
	}
	else if(head.direction == down)
	{
		head.x++;
	}
	else if(head.direction == left)
	{
		head.y--;
	}
	else if(head.direction == right)
	{
		head.y++;
	}
}

void snake_check_border()
{
	if(head.x < 0 || head.x > 8 || head.y < 0 || head.y > 18)
		snake_start();
}

void snake_set_matrix()
{
	for(uint8_t i = 0; i < 18; i++)
		for(uint8_t j = 0; j < 9; j++)
			matrix[j][i] = 0;

	matrix[head.x][head.y] = 1;
}

void snake_get_matrix(uint8_t value[9][18])
{
	for(uint8_t i = 0; i < 18; i++)
		for(uint8_t j = 0; j < 9; j++)
			value[j][i] = matrix[j][i];
}

void snake_update()
{
	snake_move();
	snake_check_border();
	snake_set_matrix();
}
