/*
 * runTest.c
 *
 *  Created on: Mar 12, 2026
 *      Author: nateh
 */


#include <stdio.h>
#include "SD_Commands.h"
#include "MAX_Commands.h"
#include "main.h"
#include "displayText.h"

double runTest(int deltaTime, int deltaTemp, int heater){

	int samplesLeft;
	const int hz = 10;
	float tempStart; //temperature at start
	double tempNow; //difference between A & B
	double tempA = 0; //Sensor A
	double tempB = 0; //Sensor B
	double runDeltaTemp; //change from start
	uint32_t startTime;
	uint32_t currentTime;
	float currentTemp;

	MAX_INITs(&hspi2);
	startTime = HAL_GetTick();
	tempStart = readTemp();
	HAL_TIM_Base_Start_IT(&htim2);

	samplesLeft = deltaTime * hz; //samples per second * seconds = samples

	METADATA md;
	char* filename = "TESTTESTTEST";
	createMeasurementFile(&filename, &md);	// note starting location on SD

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

	while ((samplesLeft > 0) && (deltaTemp > runDeltaTemp)){
		if(TEMP_TIMER){
			TEMP_TIMER = 0;
			currentTemp = readTemp();
			runDeltaTemp = currentTemp - tempStart;
			currentTime = HAL_GetTick() - startTime;
			appendTemp(filename, runDeltaTemp, currentTime);
			displayText(currentTemp, 1);
			samplesLeft--;
		}
	}
	char buf[64];
	sprintf(buf, "%.4f", (float)currentTime * 1000);
	updateMetaData(filename, META_REGION_END, buf);

	HAL_TIM_Base_Stop_IT(&htim2);
	HAL_GPIO_WritePin(EXCIT1_GPIO_Port, EXCIT1_Pin, 0); //Turn off heaters
	HAL_GPIO_WritePin(EXCIT2_GPIO_Port, EXCIT2_Pin, 0);
	return 0;
}
