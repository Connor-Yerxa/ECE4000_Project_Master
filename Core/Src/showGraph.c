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
	switch(heater){ 	//Turn on heater
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
