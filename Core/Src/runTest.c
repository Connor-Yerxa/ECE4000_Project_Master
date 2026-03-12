/*
 * runTest.c
 *
 *  Created on: Mar 12, 2026
 *      Author: nateh
 */


#include <stdio.h>

void runTest(int deltaTime, int deltaTemp, int heater){

	int samplesLeft;
	const int hz = 15;
	double tempStart; //temperature at start
	double tempNow; //difference between A & B
	double tempA = 0; //Sensor A
	double tempB = 0; //Sensor B
	double runDeltaTemp; //change from start

	// tempA = READ FUNCTION;
	// tempB = READ FUNCTION;
	tempStart = tempA - tempB;// READ START TEMPERATURE
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
	//assign name to sample (read location & time)

	//return start location, length and ?
}
