/*
 * runTest.c
 *
 *  Created on: Mar 12, 2026
 *      Author: nateh
 */


#include <stdio.h>
#include "SD_Commands.h"

double runTest(int deltaTime, int deltaTemp, int heater){

	int samplesLeft;
	const int hz = 10;
	double tempStart; //temperature at start
	double tempNow; //difference between A & B
	double tempA = 0; //Sensor A
	double tempB = 0; //Sensor B
	double runDeltaTemp; //change from start
	double k_TC;// thermal conductivity

	// tempA = READ FUNCTION;
	// tempB = READ FUNCTION;
	tempStart = tempA - tempB;// READ START TEMPERATURE no, just temo
	samplesLeft = deltaTime * hz; //samples per second * seconds = samples

	// note starting location on SD

	//Turn on heater

	while ((samplesLeft > 0) && (deltaTemp > runDeltaTemp)){
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
	return k_TC;
}
