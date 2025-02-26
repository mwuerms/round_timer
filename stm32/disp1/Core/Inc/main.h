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
#include "stm32l4xx_hal.h"

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
#define BTN3_Pin GPIO_PIN_13
#define BTN3_GPIO_Port GPIOC
#define SOUND_TX_Pin GPIO_PIN_2
#define SOUND_TX_GPIO_Port GPIOA
#define SOUND_RX_Pin GPIO_PIN_3
#define SOUND_RX_GPIO_Port GPIOA
#define LCD_SCK_Pin GPIO_PIN_5
#define LCD_SCK_GPIO_Port GPIOA
#define LCD_MISO_Pin GPIO_PIN_6
#define LCD_MISO_GPIO_Port GPIOA
#define ROT_A_Pin GPIO_PIN_0
#define ROT_A_GPIO_Port GPIOB
#define ROT_B_Pin GPIO_PIN_1
#define ROT_B_GPIO_Port GPIOB
#define DISP_RST_Pin GPIO_PIN_10
#define DISP_RST_GPIO_Port GPIOB
#define DISP_DC_Pin GPIO_PIN_11
#define DISP_DC_GPIO_Port GPIOB
#define DISP_CS_Pin GPIO_PIN_12
#define DISP_CS_GPIO_Port GPIOB
#define DISP_SCK_Pin GPIO_PIN_13
#define DISP_SCK_GPIO_Port GPIOB
#define DISP_MISO_Pin GPIO_PIN_14
#define DISP_MISO_GPIO_Port GPIOB
#define DISP_MOSI_Pin GPIO_PIN_15
#define DISP_MOSI_GPIO_Port GPIOB
#define BTN2_Pin GPIO_PIN_3
#define BTN2_GPIO_Port GPIOB
#define BTN1_Pin GPIO_PIN_4
#define BTN1_GPIO_Port GPIOB
#define ENC_IN1_Pin GPIO_PIN_5
#define ENC_IN1_GPIO_Port GPIOB
#define BTN0_Pin GPIO_PIN_6
#define BTN0_GPIO_Port GPIOB
#define BTN0_EXTI_IRQn EXTI9_5_IRQn
#define ENC_IN2_Pin GPIO_PIN_7
#define ENC_IN2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
