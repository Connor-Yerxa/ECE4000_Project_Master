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

double runTest(int deltaTime, int deltaTemp, int heater){

	int samplesLeft;
	const int hz = 10;
	double tempStart; //temperature at start
	double tempNow; //difference between A & B
	double tempA = 0; //Sensor A
	double tempB = 0; //Sensor B
	double runDeltaTemp; //change from start
	double k_TC;// thermal conductivity
	uint32_t startTime;

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


		}
		//READ TEMP A & B

		//PROCESS A SAMPLE
		tempNow = tempA - tempB; //Get current temp
		//Cold junction compensation
		runDeltaTemp = tempStart - tempNow;

		//Put sample on card at next available spot

		samplesLeft--;
	}
	//get metadata
	//WriteMetaData()

	//return start location, length and ?

	HAL_GPIO_WritePin(EXCIT1_GPIO_Port, EXCIT1_Pin, 0); //Turn off heaters
	HAL_GPIO_WritePin(EXCIT2_GPIO_Port, EXCIT2_Pin, 0);
	return k_TC;
}
