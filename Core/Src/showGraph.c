/*
 * showGraph.c
 *
 *  Created on: Mar 12, 2026
 *      Author: nateh
 */
#include "SD_Commands.h"
#include "main.h"
#include "displayText.h"

void showGraph(){
	char *filename = "TESTTESTTEST.csv";
	char * bufs;

	bufs = getMetaData(META_REGION_START);
	float startTime = atof(bufs);
	bufs = getMetaData(META_REGION_END);
	float stopTime = atof(bufs);
	float k = calculateK(startTime, stopTime, filename); //needs 3 arguments

	displayText(startTime,1);
	displayText(stopTime,1);
	displayText(k,1);
	// Plot the graph
	// Log the graph
	// Find the 2 points
	// use those 2 points & values for the calculation of k
	// return a k value
}
