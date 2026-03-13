/*
 * runCalibration.c
 *
 *  Created on: Mar 12, 2026
 *      Author: nateh
 */

#include <stdio.h>
#include "runTest.h"
#include "main.h"
#include "SD_Commands.h"
#include "Menus.h"
//#include "deleteTest.h"



void runCalibration(int type){
	double k2;
	double k1;

	/*should grab the same data as showgraph..?*/
	char * bufs;

	SDMOUNT(&hspi1);

	bufs = getMetaData(filename, META_REGION_START);
	float startTime = atof(bufs);
	bufs = getMetaData(filename, META_REGION_END);
	float stopTime = atof(bufs);

	float power = 0.1;

	calCoef = 1;
	k2 = runTest(180, 100, 1); //run a test MIGHT NEED TO CHANGE k2 later, as currently runTest returns 0
	k2 = calculateK(startTime, stopTime, filename, power); //recalculating k2 as technically runTest doesn't calculate yet.

	//	deleteTest(); //delete the test

	if (type == 1){
		k1 = 0.598; // W/m*k WATER at 20 c
	}
	else{
		k1 = 0.285; //GLYCERIN at 26.8 c
	}
	calCoef = k1/k2;
}
