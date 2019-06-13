/*
 * terminal.c
 *
 *  Created on: 13.06.2019
 *      Author: MT-LAB
 */

#include <string.h>

#include "usart.h"

uint8_t uart_tx_dma_ready = 1;

uint8_t pData_rx[10] = {0};
uint8_t pData_tx[10] = {0};

void terminal_echo_start()
{
	HAL_UART_Receive_DMA(&huart2, pData_rx, sizeof(pData_rx));
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
		memcpy(pData_tx, pData_rx, sizeof(pData_tx));


//		HAL_UART_Receive_DMA(&huart2, pData_rx, sizeof(pData_rx));
		HAL_UART_Transmit_DMA(&huart2, pData_tx, sizeof(pData_tx));

	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	HAL_UART_AbortTransmit(huart);
	uart_tx_dma_ready = 1;
}
