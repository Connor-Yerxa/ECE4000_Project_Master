/*
 * runCalibration.c
 *
 *  Created on: Mar 12, 2026
 *      Author: nateh
 */

#include <stdio.h>
#include "runTest.h"
#include "main.h"
//#include "deleteTest.h"

double calCoef; // calibration coefficent

int runCalibration(int type){
	double k2;
	double k1;

	k2 = runTest(180, 100, 1); //run a test MIGHT NEED TO CHANGE k2 later, as currently runTest returns 0
	k2 = calcultateK(); //recalculating k2 as technically runTest doesn't calculate yet.
	//	deleteTest(); //delete the test

	if (type == 1){
		k1 = 0.598; // W/m*k WATER at 20 c
	}
	else{
		//have some alteration to the runTest process
		k1 = 0.285; //GLYCERIN at 26.8 c
	}
	calCoef = k1/k2;

	return calCoef;
}
