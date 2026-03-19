/*
 * showGraph.c
 *
 *  Created on: Mar 12, 2026
 *      Author: nateh
 */
#include "SD_Commands.h"
#include "main.h"
#include "displayText.h"
#include "menus.h"
#include "showGraph.h"
#include "Menus.h"

float showGraph(){
	char * bufs;

	SDMOUNT(&hspi1);

	bufs = getMetaData(filename, META_REGION_START);
	float startTime = atof(bufs);
	bufs = getMetaData(filename, META_REGION_END);
	float stopTime = atof(bufs);
	float power;
	switch(heater){
		case 1: //0.1
			power = 0.1;
			break;
		case 2: //0.27
			power = 0.27;
			break;
		case 3: //0.5
			power = 0.5;
			break;
		default:
			power = 0;
			break;

		}
	float k = calculateK(startTime, stopTime, filename, power); //needs 3 arguments

	sd_unmount();
	displayText(startTime,1);
	displayText(stopTime,1);
	displayText(k,1);
	// Plot the graph
	// Log the graph
	// Find the 2 points
	// use those 2 points & values for the calculation of k
	return k;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

void get_boundries(float * minTime, float * maxTime, float *minTemp, float * maxTemp)
{
	FIL file;
	FRESULT fin = f_open(&file, filename, FA_READ);
	if(fin != FR_OK) printf("Couln\'t open: %s", filename);

	float time, lnTime, temp;

	char line[64];

	while(f_gets((TCHAR*)line, 64, &file) != 0 && !strstr(line, "Delta Temperature (degC)"));

	f_gets((TCHAR*)line, 64, &file);
	sscanf(line, "%f, %f, %f", &time, &lnTime, &temp);
	*minTime = lnTime;
	*maxTime = lnTime;

	*minTemp = temp;
	*maxTemp = temp;

	while(f_gets((TCHAR*)line, 64, &file) != 0)
	{
		if(sscanf(line, "%f, %f, %f", &time, &lnTime, &temp) == 3)
		{
			if(*minTime > lnTime) *minTime = lnTime;
			if(*maxTime < lnTime) *maxTime = lnTime;

			if(*minTemp > temp) *minTemp = temp;
			if(*maxTemp > temp) *maxTemp = temp;
		}
	}

	f_close(&file);
}

void drawAxis(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, float maxtemp, float minLnTime, float MaxLnTime)
{
	Displ_Line(x0, y0, x0, height, WHITE);
	Displ_Line(x0, height, width, height, WHITE);
}

void drawGraph(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height)
{
	float minTime, maxTime, minTemp, maxTemp;
	get_boundries(&minTime, &maxTime, &minTemp, &maxTemp);

	drawAxis(x0, y0, width, height, maxTemp, minTime, maxTime);

	//per pixel counts
	float lnTimePerPixel = (maxTime - minTime) / width;
	float TempPerPixel = maxTemp / height; // Ignoring negative values on screen for now.

	FIL file;
	FRESULT fin = f_open(&file, filename, FA_READ);
	if(fin != FR_OK) printf("Couln\'t open: %s", filename);

	char line[64];

	while(f_gets((TCHAR*)line, 64, &file) != 0 && !strstr(line, "Delta Temperature (degC)"));

	float xVal, t, currentTime=0, currentTemp=0;
	float timeStep = lnTimePerPixel;

	while(currentTime < maxTime)
	{
		int averageCount = 0;
		float averageTemp = 0;

		while(currentTime < timeStep)
		{

			if(f_gets((TCHAR*)line, 64, &file) == 0) break;

			if(sscanf(line, "%f, %f, %f", &t, &currentTime, &currentTemp) == 3)
			{
				if(currentTime < timeStep)
				{
					if(averageCount == 0)
					{
						xVal = currentTime;
					}
					averageCount++;
					averageTemp += currentTemp;
				}
			}
		}
		averageTemp /= (float)averageCount;
		//print pixel
		uint16_t xpoint = (uint16_t)(xVal/lnTimePerPixel) + x0;
		uint16_t ypoint = y0 + height - (uint16_t)(averageTemp/TempPerPixel);
		Displ_Pixel(xpoint, ypoint, BLUE);

		timeStep += lnTimePerPixel;
	}
}

