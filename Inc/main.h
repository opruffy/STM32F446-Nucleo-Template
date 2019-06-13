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
  * COPYRIGHT(c) 2019 STMicroelectronics
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
#include "stm32f4xx_ll_tim.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_cortex.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_utils.h"
#include "stm32f4xx_ll_pwr.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_dma.h"

#include "stm32f4xx_ll_exti.h"

/* USER CODE BEGIN Includes */
#include <inttypes.h>
/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/
#define PERIOD 200
#define PRESCALER 49

#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define LED1_Pin GPIO_PIN_2
#define LED1_GPIO_Port GPIOC
#define RPM_Pin GPIO_PIN_3
#define RPM_GPIO_Port GPIOC
#define RPM_EXTI_IRQn EXTI3_IRQn
#define LED_1_Pin GPIO_PIN_5
#define LED_1_GPIO_Port GPIOA
#define LED_2_Pin GPIO_PIN_5
#define LED_2_GPIO_Port GPIOC
#define LED_9_Pin GPIO_PIN_12
#define LED_9_GPIO_Port GPIOB
#define P3_EN_Pin GPIO_PIN_13
#define P3_EN_GPIO_Port GPIOB
#define P2_EN_Pin GPIO_PIN_14
#define P2_EN_GPIO_Port GPIOB
#define P1_EN_Pin GPIO_PIN_15
#define P1_EN_GPIO_Port GPIOB
#define P1_COMP_Pin GPIO_PIN_6
#define P1_COMP_GPIO_Port GPIOC
#define P1_COMP_EXTI_IRQn EXTI9_5_IRQn
#define P2_COMP_Pin GPIO_PIN_8
#define P2_COMP_GPIO_Port GPIOC
#define P2_COMP_EXTI_IRQn EXTI9_5_IRQn
#define P3_COMP_Pin GPIO_PIN_9
#define P3_COMP_GPIO_Port GPIOC
#define P3_COMP_EXTI_IRQn EXTI9_5_IRQn
#define SWITCH_Pin GPIO_PIN_12
#define SWITCH_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define LED_4_Pin GPIO_PIN_15
#define LED_4_GPIO_Port GPIOA
#define LED_8_Pin GPIO_PIN_10
#define LED_8_GPIO_Port GPIOC
#define LED_7_Pin GPIO_PIN_11
#define LED_7_GPIO_Port GPIOC
#define LED_6_Pin GPIO_PIN_12
#define LED_6_GPIO_Port GPIOC
#define LED_5_Pin GPIO_PIN_2
#define LED_5_GPIO_Port GPIOD
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define LED_3_Pin GPIO_PIN_7
#define LED_3_GPIO_Port GPIOB

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
