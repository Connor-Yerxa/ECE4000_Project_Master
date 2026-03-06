/*
 * GPS.h
 *
 *  Created on: Mar 4, 2026
 *      Author: cbyer
 */

#ifndef INC_GPS_H_
#define INC_GPS_H_

#include "stm32f1xx_hal.h"
#include "main.h"
#include <stdint.h>
#include <integer.h>

#define GPS_DMA_BUF_SIZE   512
#define GPS_LINE_BUF_SIZE  120

#define GPS_WAKE_HIGH()  HAL_GPIO_WritePin(GPS_Wake_GPIO_Port, GPS_Wake_Pin, GPIO_PIN_SET)
#define GPS_WAKE_LOW() HAL_GPIO_WritePin(GPS_Wake_GPIO_Port, GPS_Wake_Pin, GPIO_PIN_RESET)


typedef struct {
    float latitude;      // decimal degrees
    float longitude;     // decimal degrees
    uint32_t utc_time;   // hhmmss
    uint32_t utc_date;   // ddmmyy
    uint8_t valid;       // 1 = fix valid
    DWORD full_timestamp;
} GPS_Data_t;

extern uint8_t gps_dma_buf[GPS_DMA_BUF_SIZE];
extern GPS_Data_t gps_data;
extern uint8_t TIME_ZONE;
extern uint16_t GPS_WAIT;

void GPS_Off_On(uint8_t state);
void calc_timestamp();
void GPS_default();
void GPS_Init(UART_HandleTypeDef *huart);
void GPS_Process();
void printGPSData();
void GPS_oneshot();

#endif
