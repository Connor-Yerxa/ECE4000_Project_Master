/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "GPS.h"
#include <math.h>
#include "fonts.h"
#include "z_displ_ILI9XXX.h"
#include "z_displ_ILI9XXX_test.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern volatile uint8_t buttons;
extern volatile uint8_t TEMP_TIMER;
extern char filename[];
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
extern TIM_HandleTypeDef htim2;
extern SPI_HandleTypeDef hspi2;
extern SPI_HandleTypeDef hspi1;
//extern
//extern
//extern
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B2_Pin GPIO_PIN_13
#define B2_GPIO_Port GPIOC
#define B2_EXTI_IRQn EXTI15_10_IRQn
#define DISPL_LED_Pin GPIO_PIN_0
#define DISPL_LED_GPIO_Port GPIOC
#define DISPL_DC_Pin GPIO_PIN_1
#define DISPL_DC_GPIO_Port GPIOC
#define DISPL_RST_Pin GPIO_PIN_2
#define DISPL_RST_GPIO_Port GPIOC
#define DISPL_CS_Pin GPIO_PIN_3
#define DISPL_CS_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define THERM_CS_Pin GPIO_PIN_4
#define THERM_CS_GPIO_Port GPIOA
#define DISPL_SCK_Pin GPIO_PIN_5
#define DISPL_SCK_GPIO_Port GPIOA
#define TOUCH_MISO_Pin GPIO_PIN_6
#define TOUCH_MISO_GPIO_Port GPIOA
#define DISPL_MOSI_Pin GPIO_PIN_7
#define DISPL_MOSI_GPIO_Port GPIOA
#define B3_Pin GPIO_PIN_4
#define B3_GPIO_Port GPIOC
#define B3_EXTI_IRQn EXTI4_IRQn
#define RTD_CS_Pin GPIO_PIN_5
#define RTD_CS_GPIO_Port GPIOC
#define EXCIT1_Pin GPIO_PIN_2
#define EXCIT1_GPIO_Port GPIOB
#define B5_Pin GPIO_PIN_12
#define B5_GPIO_Port GPIOB
#define B5_EXTI_IRQn EXTI15_10_IRQn
#define B4_Pin GPIO_PIN_6
#define B4_GPIO_Port GPIOC
#define B4_EXTI_IRQn EXTI9_5_IRQn
#define EXCIT2_Pin GPIO_PIN_7
#define EXCIT2_GPIO_Port GPIOC
#define SD_CS_Pin GPIO_PIN_8
#define SD_CS_GPIO_Port GPIOC
#define GPS_Wake_Pin GPIO_PIN_9
#define GPS_Wake_GPIO_Port GPIOC
#define B6_Pin GPIO_PIN_8
#define B6_GPIO_Port GPIOA
#define B6_EXTI_IRQn EXTI9_5_IRQn
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define B1_Pin GPIO_PIN_2
#define B1_GPIO_Port GPIOD
#define B1_EXTI_IRQn EXTI2_IRQn

/* USER CODE BEGIN Private defines */
int SDMOUNT(SPI_HandleTypeDef *hspi);
void UI_DrawLine(uint8_t line, const char *text, uint8_t val);
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
