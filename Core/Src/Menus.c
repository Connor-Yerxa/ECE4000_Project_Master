/*
 * Menus.c
 *
 *  Created on: Mar 10, 2026
 *      Author: nateh
 */

#include <stdio.h>
#include <string.h>
#include "main.h"
#include "Menus.h"
#include "read_Buttons.h"
#include "selectNumber.h"
#include "displayText.h"
#include "selectNumber.h"
#include "runTest.h"
#include "runCalibration.h"
#include "showGraph.h"
#include "SD_COmmands.h"

// variable setup
int screen = 99; // main menu
int button = 0;
int number = 0;
double deltaTemp = 100;
double deltaTime = 180;
double deltaTempDefault = 100;
double deltaTimeDefault = 180;
int brightness = 10;
int both = 0;
int heater = 0;
int heaterDefault = 0;
float calCoef = 1; // calibration coefficent

void menus() {
	sprintf(filename, "TESTTESTTEST.csv");
    while(1) {
        switch(screen) {

            // ---------- MAIN MENU ----------
            case 99:
                displayText(screen, 0);
                button = read_buttons();
                switch(button) {
                    case 1: screen = 100; break; // Testing
                    case 2: screen = 200; break; // Calibration
                    case 3: screen = 300; break; // Settings
                    case 4: screen = 400; break; // Data
                    default: screen = 99; break;
                }
                break;

            // ---------- TEST MENU ----------
            case 100:
                both = 0;
                displayText(screen, 0);
                button = read_buttons();
                switch(button) {
                    case 1: // Default
                        deltaTemp = deltaTempDefault;
                        deltaTime = deltaTimeDefault;
                        heater = heaterDefault;
                        screen = 120;
                        break;
                    case 2: screen = 110; break; // Temp select
                    case 3: both = 1; screen = 110; break; // Both
                    case 4: screen = 110; break; // Time select
                    case 5: screen = 115; break; //heater selection
                    case 6: screen = 99; break; // Back to main
                    default: break;
                }
                break;

            case 110: // Temp
                displayText(screen, 0);
                number = selectNumber('E', 0);
                deltaTime = number;
                if (both == 1){
                	screen = 111; break;
                }
                screen = 120;
                break;

            case 111: // Time
                displayText(screen, 0);
                number = selectNumber('I', 0);
                deltaTemp = number;
                screen = 120;
                break;

            case 115: //heater selection
            	displayText(screen, 0);
            	button = read_buttons();
            	switch(button){
            		case 1: heater = 1; break;
            		case 2: heater = 2; break;
            		case 3: heater = 3; break;
            		default: heater = 1; break;
            	}
            	screen = 100;
            	break;

            case 120: // Test running
                displayText(screen, 0);
                runTest(deltaTemp, deltaTime, heater);
                screen = 130;
                break;

            case 130: // Test finished
                displayText(screen, 0);
                button = read_buttons();
                if (button == 1){
                	float k = showGraph();
                	char buf[16];
                	sprintf(buf, "%.5f", k);
                	updateMetaData(filename, META_CONDUCTIVITY, buf);
                	break;
                }
                else if (button == 2){
//             		deleteTest();
                }
                screen = 99; // Return to main menu
                break;

            // ---------- CALIBRATION MENU----------
            case 200:
                displayText(screen, 0);
                button = read_buttons();
                switch(button) {
                    case 1: screen = 210; break;
                    case 2: /* set default calibration */ break;
                    case 6: screen = 99; break;
                }
                break;

            case 210: // Material select
                displayText(screen, 0);
                button = read_buttons();
                switch(button) {
                    case 1: runCalibration(1); break; /* Material A calibration */
                    case 2: runCalibration(2); break; /* Material B calibration */
                    case 6: screen = 200; break;
                }
                break;

            case 220: // Cancel (future)
                displayText(screen, 0);
                if (read_buttons() == 6) screen = 210;
                break;

            case 230: // Return
                displayText(screen, 0);
                if (read_buttons()) screen = 200;
                break;

            // ---------- SETTINGS ----------
            case 300:
                displayText(screen, 0);
                button = read_buttons();
                switch(button) {
                    case 1: screen = 310; break;
                    case 2: screen = 320; break;
                    case 3: screen = 330; break;
                    case 4: screen = 340; break;
                    case 6: screen = 99; break;
                }
                break;

            case 310: // Brightness
                displayText(screen, 0);
                brightness = selectNumber('B', 0);
                screen = 300;
                break;

            case 320: // Default temperature
                displayText(screen, 0);
                deltaTempDefault = selectNumber('E', 0);
                screen = 300;
                break;

            case 330: // Default duration
                displayText(screen, 0);
                deltaTimeDefault = selectNumber('I', 0);
                screen = 300;
                break;

            case 340: //heater default power
            	displayText(screen, 0);
            	button = read_buttons();
            	switch(button){
            		case 1: heaterDefault = 1; break;
            		case 2: heaterDefault = 2; break;
            		case 3: heaterDefault = 3; break;
            		default: heaterDefault = 1; break;
            	}
            	screen = 300;
            	break;

            // ---------- OTHER ----------
            case 400:
                displayText(screen, 0);
                // Future development?
                button = read_buttons();
                switch(button) {
                    case 6: screen = 99; break;
                }
                break;


            default:
                printf("Error: screen out of range\n");
                displayText(999, 0);
                screen = 99;
                break;
        }
    }
}
