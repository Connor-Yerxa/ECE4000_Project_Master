/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file   fatfs.c
  * @brief  Code for fatfs applications
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
#include "fatfs.h"

uint8_t retUSER;    /* Return value for USER */
char USERPath[4];   /* USER logical drive path */
FATFS USERFatFS;    /* File system object for USER logical drive */
FIL USERFile;       /* File object for USER */

/* USER CODE BEGIN Variables */

#include "GPS.h"
/* USER CODE END Variables */

void MX_FATFS_Init(void)
{
  /*## FatFS: Link the USER driver ###########################*/
  retUSER = FATFS_LinkDriver(&USER_Driver, USERPath);

  /* USER CODE BEGIN Init */
  /* additional user code for init */
  /* USER CODE END Init */
}

/**
  * @brief  Gets Time from RTC
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime(void)
{
  /* USER CODE BEGIN get_fattime */
//	return ((2024 - 1980) << 25) | (3 << 21) | (5 << 16) | (12 << 11) | (0 << 5) | (0 / 2); // Default time
	uint8_t day   =  gps_data.utc_date / 10000;          	// DD
	uint8_t month = (gps_data.utc_date / 100) % 100;     	// MM
	uint8_t year  =  gps_data.utc_date % 100;            	// YY


	uint8_t hr   =  gps_data.utc_time / 10000 + TIME_ZONE;    // hr
	uint8_t min = (gps_data.utc_time / 100) % 100;			// mm
	uint8_t sec  =  gps_data.utc_time % 100;            		// ss
	return ((year + 20) << 25) | (month << 21) | (day << 16) | (hr << 11) | (min << 5) | (sec / 2);
  /* USER CODE END get_fattime */
}

/* USER CODE BEGIN Application */

/* USER CODE END Application */
