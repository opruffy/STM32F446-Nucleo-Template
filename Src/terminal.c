/*
 * terminal.c
 *
 *  Created on: 13.06.2019
 *      Author: MT-LAB
 */

#include <string.h>

#include "usart.h"

#include "clock.h"

uint8_t uart_tx_dma_ready = 1;
uint8_t uart_rx_state = 0;

uint8_t pData_rx[10] = {0};
uint8_t pData_tx[10] = {0};

uint8_t pData_uhrzeit[6] = {0};


void terminal_echo_start()
{
	HAL_UART_Receive_DMA(&huart2, pData_rx, 1);
}


void terminal_tx(uint8_t *pData, uint16_t Size)
{
	if(uart_tx_dma_ready == 1)
	{
		HAL_UART_Transmit_DMA(&huart2, pData, Size);
		uart_tx_dma_ready = 0;
	}
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart2)
	{
		if(uart_rx_state == 0)
		{
			switch (pData_rx[0]) {
			case 'u':
				HAL_UART_AbortReceive(&huart2);
				HAL_UART_Receive_DMA(&huart2, pData_rx, 6);
				uart_rx_state = 1;
				break;
			case 'd':
				HAL_UART_AbortReceive(&huart2);
				HAL_UART_Receive_DMA(&huart2, pData_rx, 6);
				uart_rx_state = 2;
				break;
			default:
				break;
			}
		}
		else if(uart_rx_state == 1)
		{
			clock_setHour(10 * (pData_rx[0] - '0') + (pData_rx[1] - '0'));
			clock_setMin(10 * (pData_rx[2] - '0') + (pData_rx[3] - '0'));
			clock_setSec(10 * (pData_rx[4] - '0') + (pData_rx[5] - '0'));

			terminal_tx(pData_rx, 6);
			HAL_UART_AbortReceive(&huart2);
			HAL_UART_Receive_DMA(&huart2, pData_rx, 1);
			uart_rx_state = 0;
		}
		else if(uart_rx_state == 2)
		{
			clock_setDay(10 * (pData_rx[0] - '0') + (pData_rx[1] - '0'));
			clock_setMonth(10 * (pData_rx[2] - '0') + (pData_rx[3] - '0'));
			clock_setYear(10 * (pData_rx[4] - '0') + (pData_rx[5] - '0'));

			terminal_tx(pData_rx, 6);
			HAL_UART_AbortReceive(&huart2);
			HAL_UART_Receive_DMA(&huart2, pData_rx, 1);
			uart_rx_state = 0;
		}


//		HAL_UART_Receive_DMA(&huart2, pData_rx, sizeof(pData_rx));
//		HAL_UART_Transmit_DMA(&huart2, pData_tx, sizeof(pData_tx));

	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	HAL_UART_AbortTransmit(huart);
	uart_tx_dma_ready = 1;
}
