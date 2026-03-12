/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "sd_functions.h"
#include "max31865_stm32.h"
#include "max31856_stm32.h"
#include "SD_Commands.h"
#include "sd_spi.h"
#include "z_displ_ILI9XXX.h"
#include <stdio.h>
#include <string.h>
#include "MAX_Commands.h"
#include "Menus.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SD_CS_LOW()     HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_RESET)
#define SD_CS_HIGH()    HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_SET)

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;
DMA_HandleTypeDef hdma_spi1_rx;
DMA_HandleTypeDef hdma_spi1_tx;

TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart1_rx;

/* USER CODE BEGIN PV */

// Faaiz UI 0/1 states
static uint8_t ui_sd_mounted   = 0;
static uint8_t ui_create_file  = 0;
static uint8_t ui_run_test     = 0;
static uint8_t ui_excit1       = 0;
static uint8_t ui_excit2       = 0;

// Faaiz screen Layout
#define UI_X      20
#define UI_Y0     40
#define UI_DY     40

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_SPI1_Init(void);
static void MX_SPI2_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
volatile uint8_t buttons = 0;

int _write(int file, char *ptr, int len) {
    for (int i = 0; i < len; i++) {
        ITM_SendChar((*ptr++));
    }
    return len;
}

int SDMOUNT(SPI_HandleTypeDef *hspi)
{
	uint32_t original = hspi->Init.BaudRatePrescaler;
	HAL_SPI_DeInit(hspi);
	hspi->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
	HAL_Delay(10);
	HAL_SPI_Init(hspi);

	SD_CS_HIGH();  // Ensure card is deselected
	HAL_Delay(10);

	int connected = sd_mount();

	HAL_SPI_DeInit(hspi);
	hspi->Init.BaudRatePrescaler = original;
	HAL_Delay(10);
	HAL_SPI_Init(hspi);

	return connected;
}

// Faaiz screen
//static void UI_DrawLine(uint8_t line, const char *text, uint8_t val)
void UI_DrawLine(uint8_t line, const char *text, uint8_t val)
{
  char buf[64];
  snprintf(buf, sizeof(buf), "%s: %d", text, val);
  Displ_WString(UI_X, (uint16_t)(UI_Y0 + line * UI_DY),
                buf, Font16, 1, WHITE, BLACK);
}

static void UI_DrawAll(void)
{
  Displ_CLS(BLACK);

  Displ_WString(UI_X, 10, "Button / Screen Test", Font16, 1, WHITE, BLACK);

  UI_DrawLine(0, "B1 - Mount SD Card",   ui_sd_mounted);
  UI_DrawLine(1, "B2 - Create file",     ui_create_file);
  UI_DrawLine(2, "B3 - Run Test",        ui_run_test);
  UI_DrawLine(3, "B4 - Exciter Relay 1", ui_excit1);
  UI_DrawLine(4, "B5 - Exciter Relay 2", ui_excit2);
}
// Faaiz screen
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
//  MAX31865_HandleTypeDef MAX_RTD;
//  MAX31865_HandleTypeDef MAX_RTD;

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */

//  SD_CS_HIGH();  // Ensure card is deselected
  HAL_Delay(30);
//  int connected = sd_mount();
//  sd_test_read_raw();
//  sd_write_file("test1.txt", "hello from STM32\r\n");

//////  char buf[64];
//////  UINT br;
//////  FRESULT r = sd_read_file("test1.txt", buf, sizeof(buf), &br);
//////  printf("read_file = %d, br = %u, data = '%s'\r\n", r, br, buf);
////
//////  sd_list_files();
////
////  //TLS-CRC-2025-10-30-11-29-14A.csv
////  readMeasurementData("readMeasurementData.csv", 6485);
////  sd_unmount();
//    GPS_Init(&huart1);
//
//  int connected = SDMOUNT(&hspi1);
//  printf("Connected: %d\n", connected);
////  sd_test_read_raw();
////  sd_write_file("test1.txt", "hello from STM32\r\n");
//
////  char buf[64];
////  UINT br;
////  FRESULT r = sd_read_file("test1.txt", buf, sizeof(buf), &br);
////  printf("read_file = %d, br = %u, data = '%s'\r\n", r, br, buf);
//
//  sd_list_files();
//
//  //TLS-CRC-2025-10-30-11-29-14A.csv - 6485
////  readMeasurementData("TLS-CRC-2025-10-30-11-29-14A.csv", &tempsLen, 15);
//
//  METADATA md;
//  char * filename = "TLS-SIN";
//  createMeasurementFile(&filename, &md);
//
////  float mins = 4;
//  float mins = 0.02;
//  float t=0;
//  float sampleTime = 1.0/15.0;
//  char text[10];
//  while(t < mins * 60)
//  {
//	  sprintf(text, "%.3f\n", sin(t));
//	  sd_append_file(filename, text);
//	  t += sampleTime;
//  }
//
//  sd_unmount();

//  GPS_Data_t gps = {0};
//  GPS_oneshot(&gps);
//  printGPSData(&gps);

//  printf("Running RTD Test...\n");
//  MAX31865_Init(&MAX_RTD, &hspi2, RTD_CS_GPIO_Port, RTD_CS_Pin, MAX31865_WIRES_3, 0);
//
//  uint16_t raw15;
//  MAX31865_ReadRTDRaw(&MAX_RTD, &raw15);
//  uint8_t buf[8];
//  MAX31865_ReadN(&MAX_RTD, 0, buf, 8);
//
//
//
//  for(int i=0;i<8;i++)
//  {
//	  printf("%02X  ", buf[i]);
//  }
//  printf("\n");

  // Faaiz screen
  // Display init
  Displ_Init(Displ_Orientat_90);
  Displ_CLS(BLACK);
  Displ_BackLight('I');
  HAL_GPIO_WritePin(DISPL_LED_GPIO_Port, DISPL_LED_Pin, 1);
  // Draw the page once
  UI_DrawAll();
  // Faaiz screen

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  menus();
	 // read_buttons();
//	  uint8_t i=buttons;
//	  printf("b: %x \n", buttons);
//	  HAL_Delay(30);
	  //buttons = 0;

	  if (buttons & 0x01) { ui_sd_mounted  ^= 1; UI_DrawLine(0, "B1 - Mount SD Card",   ui_sd_mounted); buttons = 0;}
	  if (buttons & 0x02) { ui_create_file ^= 1; UI_DrawLine(1, "B2 - Create file",     ui_create_file); buttons = 0;}
	  if (buttons & 0x04) { ui_run_test    ^= 1; UI_DrawLine(2, "B3 - Run Test",        ui_run_test); buttons = 0;}
	  if (buttons & 0x08) { ui_excit1      ^= 1; UI_DrawLine(3, "B4 - Exciter Relay 1", ui_excit1); buttons = 0;}
	  if (buttons & 0x10) { ui_excit2      ^= 1; UI_DrawLine(4, "B5 - Exciter Relay 2", ui_excit2); buttons = 0;}

	  // (PB-6 bit 0x20 ignored for now)

	  HAL_Delay(5);

//	  printf("b: %02x \n", buttons);
//	  HAL_Delay(30);
//	  buttons = 0;

//	  float temp=readTemp();
//	  printf("Temp: %2.3f\n", temp);
//	  HAL_Delay(500);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);
  /* DMA1_Channel3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);
  /* DMA1_Channel5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, DISPL_LED_Pin|DISPL_DC_Pin|DISPL_RST_Pin|EXCIT2_Pin
                          |GPS_Wake_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, DISPL_CS_Pin|RTD_CS_Pin|SD_CS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(THERM_CS_GPIO_Port, THERM_CS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(EXCIT1_GPIO_Port, EXCIT1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : B2_Pin B3_Pin B4_Pin */
  GPIO_InitStruct.Pin = B2_Pin|B3_Pin|B4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : DISPL_LED_Pin DISPL_DC_Pin DISPL_RST_Pin DISPL_CS_Pin
                           EXCIT2_Pin GPS_Wake_Pin */
  GPIO_InitStruct.Pin = DISPL_LED_Pin|DISPL_DC_Pin|DISPL_RST_Pin|DISPL_CS_Pin
                          |EXCIT2_Pin|GPS_Wake_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : THERM_CS_Pin */
  GPIO_InitStruct.Pin = THERM_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(THERM_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : RTD_CS_Pin SD_CS_Pin */
  GPIO_InitStruct.Pin = RTD_CS_Pin|SD_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : EXCIT1_Pin */
  GPIO_InitStruct.Pin = EXCIT1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(EXCIT1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : B5_Pin */
  GPIO_InitStruct.Pin = B5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B5_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : B6_Pin */
  GPIO_InitStruct.Pin = B6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B6_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == B1_Pin)
  {
	  buttons |= 0x01;
  }
  if(GPIO_Pin == B2_Pin)
  {
	  buttons |= 0x02;
  }
  if(GPIO_Pin == B3_Pin)
  {
	  buttons |= 0x04;
  }
  if(GPIO_Pin == B4_Pin)
  {
	  buttons |= 0x08;
  }
  if(GPIO_Pin == B5_Pin)
  {
	  buttons |= 0x10;
  }
  if(GPIO_Pin == B6_Pin)
  {
	  buttons |= 0x20;
  }
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    SD_OnSpiTxComplete(hspi);
    Displ_OnSpiTxComplete(hspi);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
