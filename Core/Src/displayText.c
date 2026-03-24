/*
 * displayText.c
 *
 *  Created on: Mar 10, 2026
 *      Author: nateh
 */


#include <stdio.h>
#include "main.h"
//#include <SPI.h>
//#include <Adafruit_GFX.h>
//#include <adafruit_ILI9488.h>


// Example displayText function

// OPTION 0 = SCREEN, OPTION 1 = NUMBER

char text[6][30];   // 6 buttons, 20 chars each
char display[60];   // main display text
char buffer[60];

void displayText(int ID,  int extra)
{
	// Clear previous text
	strcpy(display, "");
	for(int i = 0; i < 6; i++)
	    strcpy(text[i], "");

	if (extra == 0){
		switch(ID)
		{
		case 99:
				strcpy(display, "Menu");
				strcpy(text[0], "Run Test");
				strcpy(text[1], "Calibration");
				strcpy(text[2], "Settings");
				strcpy(text[4], "Graph");
				break;

		case 100:
				strcpy(display, "Select testing limit");
				strcpy(text[0], "Default");
				strcpy(text[1], "Temperature");
				strcpy(text[2], "Both");
				strcpy(text[3], "Time");
				strcpy(text[4], "Change Heater Selection");
				strcpy(text[5], "Return");
				break;

		case 110:
				strcpy(display, "Select time limit in sec, max: 180"); //NUMBER SELECT
				break;

		case 111:
				strcpy(display, "Select maximum temp change (xx.x), max: 10.0"); //NUMBER SELECT
				break;
		case 115:
				strcpy(display, "Heater Selection"); //Number select
				strcpy(text[0], "Heater 1 - 0.1 W");
				strcpy(text[1], "Heater 2 - 0.3 W");
				strcpy(text[2], "Heater 3 - 0.5 W");
				break;

		case 120:
				strcpy(display, "Test Underway");
				strcpy(text[5], "STOP");
				break;
		case 130:
				strcpy(display, "Test Complete");
				strcpy(text[0], "Show Graph");
				strcpy(text[1], "Delete Test");
				break;

		case 200:
				strcpy(display, "Calibration");
				strcpy(text[0], "Physical Material");
				strcpy(text[1], "Default");
				strcpy(text[5], "Return");
				break;

		case 210:
				strcpy(display, "Select material");
				strcpy(text[0], "Material A - Water");
				strcpy(text[1], "Material B - Glycerin");
				strcpy(text[5], "Return");
				break;

		case 220:
				strcpy(display, "Running calibration");
				strcpy(text[5], "Cancel");
				break;

		case 230:
				strcpy(display, "Calibration complete");
				strcpy(text[0], "Press any button to continue");
				break;

		case 300:
				strcpy(display, "Settings");
				strcpy(text[0], "Brightness");
				strcpy(text[1], "Set default sample time");
				strcpy(text[2], "Set default temp delta");
				strcpy(text[3], "Set default heater selection");
				strcpy(text[5], "Return");
				break;

		case 310:
				strcpy(display, "Brightness 0-9"); //Number select
				break;

		case 320:
				strcpy(display, "Temperature change"); //Number select
				break;

		case 330:
				strcpy(display, "Sample duration"); //Number select
				break;
		case 340:
				strcpy(display, "Default Heater Selection"); //Number select
				strcpy(text[0], "Heater 1 - 0.1 W");
				strcpy(text[1], "Heater 2 - 0.3 W");
				strcpy(text[2], "Heater 3 - 0.5 W");
				break;

		case 400:
				strcpy(display, "Data");
				strcpy(text[5], "Return");
				break;

		case 401:
			strcpy(display, "Are You Sure?");
			strcpy(text[1], "YES");
			strcpy(text[4], "NO");
			break;

		case 500:
				strcpy(display, "Select File");
				strcpy(text[0], "UP");
				strcpy(text[1], "SEL");
				strcpy(text[2], "DOWN");
				strcpy(text[4], "DEL");
				strcpy(text[5], "BACK");
				break;
		case 501:
			strcpy(text[0], "LEFT");
			strcpy(text[2], "SW");
			strcpy(text[3], "RIGHT");
			strcpy(text[4], "CalcK");
			strcpy(text[5], "BACK");
			break;

		case 999:
				strcpy(display, "ERROR: out of range, returning to safe parameters");
				break;
		}

	    Displ_CLS(BLACK);

	    snprintf(buffer, sizeof(buffer), "%s", display); //UPDATE THIS TO BE TITLE?
	    UI_DrawLine(0, buffer, ID);
	    snprintf(buffer, sizeof(buffer), "%s", text[0]);
	    UI_DrawLine(1, buffer, 1);
	    snprintf(buffer, sizeof(buffer), "%s", text[1]);
	    UI_DrawLine(2, buffer, 2);
	    snprintf(buffer, sizeof(buffer), "%s", text[2]);
	    UI_DrawLine(3, buffer, 3);
	    snprintf(buffer, sizeof(buffer), "%s", text[3]);
	    UI_DrawLine(4, buffer, 4);
	    snprintf(buffer, sizeof(buffer), "%s", text[4]);
	    UI_DrawLine(5, buffer, 5);
	    snprintf(buffer, sizeof(buffer), "%s", text[5]);
	    UI_DrawLine(6, buffer, 6);
	}
	if (extra == 1){
	    Displ_CLS(BLACK);
	    strcpy(buffer, "Digit selection");
	    UI_DrawLine(5, buffer, ID);
	}
}
