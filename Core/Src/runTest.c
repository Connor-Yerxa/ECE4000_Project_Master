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
//	const int hz = 10;
	float tempStart; //temperature at start
//	double tempNow; //difference between A & B
//	double tempA = 0; //Sensor A
//	double tempB = 0; //Sensor B
	double runDeltaTemp; //change from start
	uint32_t startTime;
	uint32_t currentTime=0;
	float currentTemp;

	float power;

	char tempbuf[16];

	HAL_TIM_Base_Start_IT(&htim2);

	tempStart = readTemp();

//	samplesLeft = deltaTime * hz; //samples per second * seconds = samples
//	samplesLeft = 5 * hz; //samples per second * seconds = samples

	METADATA md;
	Displ_WString(5, 50, "Connecting to GPS...", Font16, 1, MAINTEXTCOLOUR, BACKGROUNDCOLOUR);
	printf("Connecting to GPS");
	GPS_oneshot();
	Displ_WString(5, 50+16, "GPS Connected", Font16, 1, MAINTEXTCOLOUR, BACKGROUNDCOLOUR);

	printf("Creating File.\n");
	Displ_WString(5, 50+2*16, "Creating file", Font16, 1, MAINTEXTCOLOUR, BACKGROUNDCOLOUR);
	createMeasurementFile(&md);	// note starting location on SD
	printf("FileCreated\n");

	snprintf(tempbuf, 16, "%.3f", tempStart);
	updateMetaData(filename, META_INITIAL_TEMP, tempbuf);
	Displ_WString(5, 50+3*16, "FileCreated", Font16, 1, MAINTEXTCOLOUR, BACKGROUNDCOLOUR);

//	heater = 2;

	switch(heater){ 	//Turn on heater
	case 1: //0.1 W
		HAL_GPIO_WritePin(EXCIT1_GPIO_Port, EXCIT1_Pin, 1);
		HAL_GPIO_WritePin(EXCIT2_GPIO_Port, EXCIT2_Pin, 0);
		power = 0.0970;
		break;
	case 2: //0.25 W
		HAL_GPIO_WritePin(EXCIT1_GPIO_Port, EXCIT1_Pin, 0);
		HAL_GPIO_WritePin(EXCIT2_GPIO_Port, EXCIT2_Pin, 1);
		power = 0.2309;
		break;
	case 3: //0.5 W
		HAL_GPIO_WritePin(EXCIT1_GPIO_Port, EXCIT1_Pin, 1);
		HAL_GPIO_WritePin(EXCIT2_GPIO_Port, EXCIT2_Pin, 1);
		power = 0.4518;
		break;
	default:
		HAL_GPIO_WritePin(EXCIT1_GPIO_Port, EXCIT1_Pin, 0);
		HAL_GPIO_WritePin(EXCIT2_GPIO_Port, EXCIT2_Pin, 0);
		power = 0;
		break;

	}

	snprintf(tempbuf, 16, "Delta T:");
	Displ_WString(10, 320-10-24*2, tempbuf, Font24, 1, SECONDARYTEXTCOLOUR, BACKGROUNDCOLOUR);
	snprintf(tempbuf, 16, "Time:");
	Displ_WString(10, 320-10-24, tempbuf, Font24, 1, SECONDARYTEXTCOLOUR, BACKGROUNDCOLOUR);
	snprintf(tempbuf, 16, "/%d s", deltaTime);
	Displ_WString(10 + 17*(6+5), 320-10-24, tempbuf, Font24, 1, MAINTEXTCOLOUR, BACKGROUNDCOLOUR);


	startTime = HAL_GetTick();
	HAL_Delay(10);
	while ((((float)currentTime) / 1000 < deltaTime) && (deltaTemp > runDeltaTemp)){
//	while ((((float)currentTime) / 1000 < 300) && (deltaTemp > runDeltaTemp)){
//	while ((((float)currentTime) / 1000 < 5) && (deltaTemp > runDeltaTemp)){
		if(TEMP_TIMER){
			TEMP_TIMER = 0;
			currentTemp = readTemp();
			runDeltaTemp = currentTemp - tempStart;
			currentTime = HAL_GetTick() - startTime;
			appendTemp(filename, runDeltaTemp, currentTime);

			sprintf(tempbuf, "%4.3f C  ", runDeltaTemp);
			Displ_WString(10+17*9, 320-10-24*2, tempbuf, Font24, 1, MAINTEXTCOLOUR, BACKGROUNDCOLOUR);
			sprintf(tempbuf, "%4.0f", ((float)currentTime) / 1000.0);
			Displ_WString(10+17*6, 320-10-24, tempbuf, Font24, 1, MAINTEXTCOLOUR, BACKGROUNDCOLOUR);
			samplesLeft--;

			if(buttons & 0x20)
			{
				HAL_Delay(150);
				buttons = 0;
				break;
			}
		}
	}
	HAL_TIM_Base_Stop_IT(&htim2);

	printf("Test done.\nWriting metadata...\n");
	char buf[64];
	sprintf(buf, "%.4f", 3*log((float)currentTime / 1000)/4);
	updateMetaData(filename, META_REGION_END, buf);
	sprintf(buf, "%.4f", 19*log((float)currentTime / 1000)/20);
	updateMetaData(filename, META_REGION_START, buf);

	sprintf(buf, "%.5f", gps_data.latitude);
	updateMetaData(filename, META_LATITUDE, buf);
	sprintf(buf, "%.5f", gps_data.longitude);
	updateMetaData(filename, META_LONGITUDE, buf);

	sprintf(buf, "%.4f", power);
	updateMetaData(filename, META_POWER, buf);

	printf("Writing metadata done\n");

//	sd_unmount();

	HAL_GPIO_WritePin(EXCIT1_GPIO_Port, EXCIT1_Pin, 0); //Turn off heaters
	HAL_GPIO_WritePin(EXCIT2_GPIO_Port, EXCIT2_Pin, 0);
	return 0;
}
