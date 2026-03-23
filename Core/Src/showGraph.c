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

//	SDMOUNT(&hspi1);

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

void get_boundries(float *minLn, float *maxLn, float *minTemp, float *maxTemp)
{
    FIL file;
    char line[64];
    float t, ln, temp;

    if (f_open(&file, filename, FA_READ) != FR_OK)
        return;

    // Skip header
    while (f_gets(line, sizeof(line), &file) && !strstr(line, "Delta Temperature"));

    // First data row
    if (f_gets(line, sizeof(line), &file) == 0) return;
    sscanf(line, "%f, %f, %f", &t, &ln, &temp);

    *minLn = *maxLn = ln;
    *minTemp = *maxTemp = temp;

    // Remaining rows
    while (f_gets(line, sizeof(line), &file))
    {
        if (sscanf(line, "%f, %f, %f", &t, &ln, &temp) == 3)
        {
            if (ln < *minLn) *minLn = ln;
            if (ln > *maxLn) *maxLn = ln;
            if (temp < *minTemp) *minTemp = temp;
            if (temp > *maxTemp) *maxTemp = temp;
        }
    }

    f_close(&file);
}

void drawAxis(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, float maxtemp, float minLnTime, float MaxLnTime)
{
	Displ_Line(x0, y0, x0, height, WHITE);
	Displ_Line(x0, height, width, height, WHITE);
}

//void drawGraph(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height)
//{
//	float minTime, maxTime, minTemp, maxTemp;
//	get_boundries(&minTime, &maxTime, &minTemp, &maxTemp);
//
//	printf("%.2f, %.2f, %.2f, %.2f\n", minTime, maxTime, minTemp, maxTemp);
//
//	drawAxis(x0, y0, width, height, maxTemp, minTime, maxTime);
//
//	//per pixel counts
//	float lnTimePerPixel = (maxTime - minTime) / width;
//	float TempPerPixel = maxTemp / height; // Ignoring negative values on screen for now.
//
//	FIL file;
//	FRESULT fin = f_open(&file, filename, FA_READ);
//	if(fin != FR_OK) printf("Couln\'t open: %s", filename);
//
//	char line[64];
//
//	while(f_gets((TCHAR*)line, 64, &file) != 0 && !strstr(line, "Delta Temperature (degC)"));
//
//	float xVal, t, currentTime=0, currentTemp=0;
//	float timeStep = lnTimePerPixel;
//
//	while(currentTime < maxTime)
//	{
//		int averageCount = 0;
//		float averageTemp = 0;
//
//		while(currentTime < timeStep)
//		{
//
//			if(f_gets((TCHAR*)line, 64, &file) == 0) break;
//
//			if(sscanf(line, "%f, %f, %f", &t, &currentTime, &currentTemp) == 3)
//			{
//				if(currentTime < timeStep)
//				{
//					if(averageCount == 0)
//					{
//						xVal = currentTime;
//					}
//					averageCount++;
//					averageTemp += currentTemp;
//				}
//			}
//		}
//		averageTemp /= (float)averageCount;
//		//print pixel
//		uint16_t xpoint = (uint16_t)(xVal/lnTimePerPixel) + x0;
//		uint16_t ypoint = y0 + height - (uint16_t)(averageTemp/TempPerPixel);
//		Displ_Pixel(xpoint, ypoint, CYAN);
//
//		timeStep += lnTimePerPixel;
//	}
//}

void drawGraph(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height)
{
    float minLn, maxLn, minTemp, maxTemp;
    get_boundries(&minLn, &maxLn, &minTemp, &maxTemp);

    float lnRange   = maxLn - minLn;
    float tempRange = maxTemp - minTemp;

    // Clear graph area
    Displ_FillArea(x0, y0, width, height, BLACK);

    // Draw axes
    Displ_Line(x0, y0 + height, x0 + width, y0 + height, WHITE);
    Displ_Line(x0, y0, x0, y0 + height, WHITE);

    // Open CSV again for streaming
    FIL file;
    char line[64];
    float t, ln, temp;

    if (f_open(&file, filename, FA_READ) != FR_OK)
        return;

    // Skip header
    while (f_gets(line, sizeof(line), &file) && !strstr(line, "Delta Temperature"));

    // Pixel window
    int px = 0;
    float pxMinLn = minLn;
    float pxMaxLn = minLn + lnRange / width;

    // Accumulators for averaging
    float sumTemp = 0.0f;
    int countTemp = 0;

    while (f_gets(line, sizeof(line), &file))
    {
        if (sscanf(line, "%f, %f, %f", &t, &ln, &temp) != 3)
            continue;

        // Advance pixel window until ln fits
        while (ln > pxMaxLn && px < width)
        {
            if (countTemp > 0)
            {
                float avgTemp = sumTemp / (float)countTemp;

                int y = y0 + height -
                        (int)(((avgTemp - minTemp) / tempRange) * height);

                Displ_Pixel(x0 + px, y, CYAN);
            }

            // Move to next pixel column
            px++;
            pxMinLn = minLn + (lnRange * px) / width;
            pxMaxLn = minLn + (lnRange * (px + 1)) / width;

            sumTemp = 0.0f;
            countTemp = 0;
        }

        // Accumulate for this pixel
        if (px < width)
        {
            sumTemp += temp;
            countTemp++;
        }
    }

    // Draw last pixel if needed
    if (countTemp > 0 && px < width)
    {
        float avgTemp = sumTemp / (float)countTemp;
        int y = y0 + height -
                (int)(((avgTemp - minTemp) / tempRange) * height);
        Displ_Pixel(x0 + px, y, CYAN);
    }

    f_close(&file);
}

