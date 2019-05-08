/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* USER CODE BEGIN Includes */
#include <inttypes.h>
/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

// BLDC
#define USE_BLDC
#define PERIOD_START_VALUE		100
#define PRESCALER_START_VALUE	5000

// Schritt
//#define SCHRITTMOTOR
//#define PERIOD_START_VALUE	200


/* Private GPIO defines ------------------*/
#define B1_Pin 				GPIO_PIN_6
#define B1_GPIO_Port 		GPIOC
#define B1_EXTI_IRQn 		EXTI9_5_IRQn

#define B2_Pin 				GPIO_PIN_8
#define B2_GPIO_Port 		GPIOC
#define B2_EXTI_IRQn 		EXTI9_5_IRQn

#define B3_Pin 				GPIO_PIN_9
#define B3_GPIO_Port 		GPIOC
#define B3_EXTI_IRQn 		EXTI9_5_IRQn

#define USART_TX_Pin 		GPIO_PIN_2
#define USART_TX_GPIO_Port 	GPIOA
#define USART_RX_Pin 		GPIO_PIN_3
#define USART_RX_GPIO_Port 	GPIOA


//--Test Pins
#define TEST1_Pin 			GPIO_PIN_12
#define TEST1_GPIO_Port 	GPIOB

#define TEST2_Pin 			GPIO_PIN_11
#define TEST2_GPIO_Port 	GPIOA

#define TEST3_Pin 			GPIO_PIN_12
#define TEST3_GPIO_Port 	GPIOA


#define LD2_Pin 			GPIO_PIN_5
#define LD2_GPIO_Port 		GPIOA

#define TMS_Pin 			GPIO_PIN_13
#define TMS_GPIO_Port 		GPIOA

#define TCK_Pin 			GPIO_PIN_14
#define TCK_GPIO_Port 		GPIOA

#define SWO_Pin 			GPIO_PIN_3
#define SWO_GPIO_Port 		GPIOB

#define P1_Pin 				GPIO_PIN_15
#define P1_GPIO_Port	 	GPIOB

#define P2_Pin 				GPIO_PIN_14
#define P2_GPIO_Port	 	GPIOB

#define P3_Pin 				GPIO_PIN_13
#define P3_GPIO_Port	 	GPIOB


#define S1a_Pin				GPIO_PIN_7
#define S1a_GPIO_Port	 	GPIOA

#define S1b_Pin 			GPIO_PIN_8
#define S1b_GPIO_Port	 	GPIOA

#define S2a_Pin				GPIO_PIN_9
#define S2a_GPIO_Port	 	GPIOA

#define S2b_Pin				GPIO_PIN_10
#define S2b_GPIO_Port	 	GPIOA

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */
extern volatile uint16_t blinkDelay;
/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
