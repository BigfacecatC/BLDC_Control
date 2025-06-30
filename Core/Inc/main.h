/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define YC_SD_Pin GPIO_PIN_13
#define YC_SD_GPIO_Port GPIOC
#define HALL_U_Pin GPIO_PIN_0
#define HALL_U_GPIO_Port GPIOA
#define HALL_V_Pin GPIO_PIN_1
#define HALL_V_GPIO_Port GPIOA
#define HALL_W_Pin GPIO_PIN_2
#define HALL_W_GPIO_Port GPIOA
#define PWM_U_L_Pin GPIO_PIN_7
#define PWM_U_L_GPIO_Port GPIOA
#define PWM_V_L_Pin GPIO_PIN_0
#define PWM_V_L_GPIO_Port GPIOB
#define PWM_W_L_Pin GPIO_PIN_1
#define PWM_W_L_GPIO_Port GPIOB
#define PWM_U_H_Pin GPIO_PIN_6
#define PWM_U_H_GPIO_Port GPIOC
#define PWM_V_H_Pin GPIO_PIN_7
#define PWM_V_H_GPIO_Port GPIOC
#define PWM_W_H_Pin GPIO_PIN_8
#define PWM_W_H_GPIO_Port GPIOC
#define KEY_4_Pin GPIO_PIN_2
#define KEY_4_GPIO_Port GPIOD
#define KEY_4_EXTI_IRQn EXTI2_IRQn
#define KEY_3_Pin GPIO_PIN_3
#define KEY_3_GPIO_Port GPIOB
#define KEY_3_EXTI_IRQn EXTI3_IRQn
#define KEY_2_Pin GPIO_PIN_4
#define KEY_2_GPIO_Port GPIOB
#define KEY_2_EXTI_IRQn EXTI4_IRQn
#define KEY_1_Pin GPIO_PIN_5
#define KEY_1_GPIO_Port GPIOB
#define KEY_1_EXTI_IRQn EXTI9_5_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
