/*
 * GPS.c
 *
 *  Created on: Mar 4, 2026
 *      Author: cbyer
 */
#include "GPS.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

uint8_t gps_dma_buf[GPS_DMA_BUF_SIZE];

GPS_Data_t gps_data = {0};
uint8_t TIME_ZONE = -4;
uint16_t GPS_WAIT = 500;

static UART_HandleTypeDef *gps_huart;
static uint16_t dma_idx = 0;
static char line_buf[GPS_LINE_BUF_SIZE];
static uint16_t line_pos = 0;

void GPS_Off_On(uint8_t state)
{
	switch(state)
	{
		case 0:
			GPS_WAKE_LOW();
			HAL_Delay(1000);  // long press forces shutdown
			GPS_WAKE_HIGH();
			break;
		case 1:
			GPS_WAKE_LOW();
			HAL_Delay(150);   // short press
			GPS_WAKE_HIGH();
			break;
	}
}

static float gps_convert_deg(float raw)
{
    int deg = (int)(raw / 100);
    float minutes = raw - deg * 100;
    return deg + minutes / 60.0f;
}

static void gps_parse_rmc(char *line)
{
    char *token;
    int field = 0;

    token = strtok(line, ",");
    while (token != NULL)
    {
        switch (field)
        {
            case 1: gps_data.utc_time = atoi(token); break;
            case 2: gps_data.valid = (token[0] == 'A'); break;
            case 3: gps_data.latitude = gps_convert_deg(atof(token)); break;
            case 4: if (token[0] == 'S') gps_data.latitude = - gps_data.latitude; break;
            case 5: gps_data.longitude = gps_convert_deg(atof(token)); break;
            case 6: if (token[0] == 'W') gps_data.longitude = - gps_data.longitude; break;
            case 9: gps_data.utc_date = atoi(token); break;
        }
        token = strtok(NULL, ",");
        field++;
    }
    printf("Valid: %d\n", gps_data.valid);
}

void GPS_Init(UART_HandleTypeDef *huart)
{
    gps_huart = huart;
    HAL_UART_Receive_DMA(gps_huart, gps_dma_buf, GPS_DMA_BUF_SIZE);
    GPS_Off_On(0);
}

void GPS_Process()
{
    while (1) {
        uint8_t c = gps_dma_buf[dma_idx];

        dma_idx++;
        if (dma_idx >= GPS_DMA_BUF_SIZE)
        {
            dma_idx = 0;
        }

        if (c == '\n')
        {
            line_buf[line_pos] = '\0';

            if (strncmp(line_buf, "$GNRMC", 6) == 0)
            {
                gps_parse_rmc(line_buf);
                line_pos = 0;
                break;
            } else line_pos = 0;
        } else
        {
            if (line_pos < GPS_LINE_BUF_SIZE - 1)
                line_buf[line_pos++] = c;
        }
    }
}

void printGPSData()
{
	printf("Valid: %c\n", gps_data.valid ? 'A' : 'V');

	printf("UTC-D: %6lu\n", gps_data.utc_date);
	printf("UTC-T: %6lu\n", gps_data.utc_time);
	printf("AST: %6lu\n", gps_data.utc_time + (TIME_ZONE * 10000));

	printf("Latitude: %.4f\n", gps_data.latitude);
	printf("Longitude: %.4f\n", gps_data.longitude);
}

void GPS_oneshot()
{
	GPS_Off_On(1);
	gps_data.valid = 0;
	int i=0;
	while(!gps_data.valid && i < GPS_WAIT)
	{
		GPS_Process();
//		printGPSData(gps_data);
		HAL_Delay(500);
		i++;
	}

	if(i > GPS_WAIT)
	{
		gps_data.valid = 1;
		gps_data.utc_date = 050326;
		gps_data.utc_time = 224030;
	}

	GPS_Off_On(0);
}
