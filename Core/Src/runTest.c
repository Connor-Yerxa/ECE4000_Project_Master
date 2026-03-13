/*
 * runTest.c
 *
 *  Created on: Mar 12, 2026
 *      Author: nateh
 */

#include "runTest.h"
#include <stdio.h>
#include "SD_Commands.h"
#include "MAX_Commands.h"
#include "main.h"
#include "displayText.h"
#include "GPS.h"

double runTest(int deltaTime, int deltaTemp, int heater){

	int samplesLeft;
	const int hz = 10;
	float tempStart; //temperature at start
//	double tempNow; //difference between A & B
//	double tempA = 0; //Sensor A
//	double tempB = 0; //Sensor B
	double runDeltaTemp; //change from start
	uint32_t startTime;
	uint32_t currentTime=0;
	float currentTemp;

	MAX_INITs(&hspi2);
	HAL_TIM_Base_Start_IT(&htim2);

	samplesLeft = deltaTime * hz; //samples per second * seconds = samples
	samplesLeft = 5 * hz; //samples per second * seconds = samples

	METADATA md;

	printf("Creating File.\n");
	createMeasurementFile(&md);	// note starting location on SD
	printf("FileCreated\n");

	switch(heater){ 	//Turn on heater
	case 1: //0.1
		HAL_GPIO_WritePin(EXCIT1_GPIO_Port, EXCIT1_Pin, 1);
		HAL_GPIO_WritePin(EXCIT2_GPIO_Port, EXCIT2_Pin, 0);
		break;
	case 2: //0.27
		HAL_GPIO_WritePin(EXCIT1_GPIO_Port, EXCIT1_Pin, 0);
		HAL_GPIO_WritePin(EXCIT2_GPIO_Port, EXCIT2_Pin, 1);
		break;
	case 3: //0.5
		HAL_GPIO_WritePin(EXCIT1_GPIO_Port, EXCIT1_Pin, 1);
		HAL_GPIO_WritePin(EXCIT2_GPIO_Port, EXCIT2_Pin, 1);
		break;
	default:
		HAL_GPIO_WritePin(EXCIT1_GPIO_Port, EXCIT1_Pin, 0);
		HAL_GPIO_WritePin(EXCIT2_GPIO_Port, EXCIT2_Pin, 0);
		break;

	}


	startTime = HAL_GetTick();
	tempStart = readTemp();
//	while ((((float)currentTime) / 1000 < deltaTime) && (deltaTemp > runDeltaTemp)){
	while ((((float)currentTime) / 1000 < 300) && (deltaTemp > runDeltaTemp)){
//	while ((((float)currentTime) / 1000 < 5) && (deltaTemp > runDeltaTemp)){
		if(TEMP_TIMER){
			TEMP_TIMER = 0;
			currentTemp = readTemp();
			runDeltaTemp = currentTemp - tempStart;
			currentTime = HAL_GetTick() - startTime;
			appendTemp(filename, runDeltaTemp, currentTime);

			char tempbuf[10];
			sprintf(tempbuf, "%.3f C", currentTemp);
			Displ_WString(70, 60, tempbuf, Font24, 1, WHITE, BLACK);
			sprintf(tempbuf, "%.3f s", ((float)currentTime) / 1000.0);
			Displ_WString(70, 60+24, tempbuf, Font24, 1, WHITE, BLACK);
			samplesLeft--;
		}
	}
	HAL_TIM_Base_Stop_IT(&htim2);

	printf("Test done.\nWriting metadata...\n");
	char buf[64];
	sprintf(buf, "%.4f", (float)currentTime / 1000);
	updateMetaData(filename, META_REGION_END, buf);
	sprintf(buf, "%.4f", (float)currentTime / 1000 /10);
	updateMetaData(filename, META_REGION_START, buf);

	sprintf(buf, "%.5f", gps_data.latitude);
	updateMetaData(filename, META_LATITUDE, buf);
	sprintf(buf, "%.5f", gps_data.longitude);
	updateMetaData(filename, META_LONGITUDE, buf);

	printf("Writing metadata done\n");

//	sd_unmount();

	HAL_GPIO_WritePin(EXCIT1_GPIO_Port, EXCIT1_Pin, 0); //Turn off heaters
	HAL_GPIO_WritePin(EXCIT2_GPIO_Port, EXCIT2_Pin, 0);
	return 0;
}
