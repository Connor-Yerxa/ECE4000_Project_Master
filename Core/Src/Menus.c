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
#include "SD_Commands.h"
#include "fileselecting.h"
#include "MAX_Commands.h"

// variable setup
int screen = 99; // main menu
int button = 0;
int number = 0;
double deltaTemp;
int deltaTime;
double deltaTempDefault = 10;
int deltaTimeDefault = 180;
//int brightness = 10;
int both = 0;
int heater;
int heaterDefault = 3;
float calCoef = 1; // calibration coefficent

int last_screen = 99;

uint8_t getVerification()
{
	Displ_CLS(BACKGROUNDCOLOUR);
	UI_DrawLine(0, "Are You Sure?", 0);
	UI_DrawLine(2, "YES", 0);
	UI_DrawLine(5, "NO", 0);

	HAL_Delay(1000);
	buttons = 0;

	while(1)
	{
		if(buttons & 0x02)
		{
			HAL_Delay(debounceDelay);
			buttons = 0;
			return 1;
		}
		if(buttons & 0x10)
		{
			buttons = 0;
			return 0;
		}
	}
}

void menus() {
	int ii=0;
	sprintf(filename, "TESTTESTTEST.csv");
	deltaTime = deltaTimeDefault;
	deltaTemp = deltaTempDefault;
	heater = heaterDefault;
    while(1) {
        switch(screen) {

            // ---------- MAIN MENU ----------
            case 99:
                displayText(screen, 0);
//                button = read_buttons();
//                switch(button) {
//                    case 1: last_screen = screen; screen = 100; break; // Testing
//                    case 2: last_screen = screen; screen = 200; break; // Calibration
//                    case 3: last_screen = screen; screen = 300; break; // Settings
//                    case 4: last_screen = screen; screen = 400; break; // Data
//                    case 5: last_screen = screen; screen = 500; break;
//                    default: last_screen = screen; screen = 99; break;
//                }

                int t = HAL_GetTick();
                while(1)
                {
                	if(HAL_GetTick() > t + 1000)
                	{

						char buf[32];
						float temp = readTemp();
//						printf("%.2f\n", temp);

						if(temp < -100 || temp > 200)
						{
							snprintf(buf, 32, "Current Temp: %.1f", temp);
							Displ_WString(480/2 - 11*strlen(buf)/2, 320-3*16-10, "                  ", Font16, 1, BACKGROUNDCOLOUR, BACKGROUNDCOLOUR);
							snprintf(buf, 32, "No TLS Connected");
							Displ_WString(480/2 - 11*strlen(buf)/2, 320-3*16-10, buf, Font16, 1, SECONDARYTEXTCOLOUR, BACKGROUNDCOLOUR);
						} else
						{
							snprintf(buf, 32, "No TLS Connected");
							Displ_WString(480/2 - 11*strlen(buf)/2, 320-3*16-10, "                     ", Font16, 1, BACKGROUNDCOLOUR, BACKGROUNDCOLOUR);
							snprintf(buf, 32, "Current Temp: %3.1f", temp);
							Displ_WString(480/2 - 11*strlen(buf)/2, 320-3*16-10, buf, Font16, 1, SECONDARYTEXTCOLOUR, BACKGROUNDCOLOUR);
						}

						GPS_Process();
						printf("Valid: %d\n", gps_data.valid);
						if(gps_data.valid == 1)
						{
							snprintf(buf, 32, "GPS Connecting...");
							Displ_WString(480/2 - 11*strlen(buf)/2, 320-2*16-10, buf, Font16, 1, BACKGROUNDCOLOUR, BACKGROUNDCOLOUR);
							snprintf(buf, 32, "Long: %.5f", gps_data.longitude);
							Displ_WString(480/2 - 11*strlen(buf)/2, 320-2*16-10, buf, Font16, 1, SECONDARYTEXTCOLOUR, BACKGROUNDCOLOUR);
							snprintf(buf, 32, "Lat: %.5f", gps_data.latitude);
							Displ_WString(480/2 - 11*strlen(buf)/2, 320-16-10, buf, Font16, 1, SECONDARYTEXTCOLOUR, BACKGROUNDCOLOUR);
						} else
						{
							snprintf(buf, 32, "GPS Connecting...");
							Displ_WString(480/2 - 11*strlen(buf)/2, 320-2*16-10, buf, Font16, 1, SECONDARYTEXTCOLOUR, BACKGROUNDCOLOUR);
							snprintf(buf, 32, "GPS Connecting...");
							Displ_WString(480/2 - 11*strlen(buf)/2, 320-16-10, buf, Font16, 1, BACKGROUNDCOLOUR, BACKGROUNDCOLOUR);
						}

						t = HAL_GetTick();
                	}



                	if(buttons & 0x01)
					{
                		HAL_Delay(debounceDelay);
                		buttons = 0;
                		last_screen = screen;
                		screen = 100; // Testing
                		break;
					} else if(buttons & 0x02)
					{
                		HAL_Delay(debounceDelay);
                		buttons = 0;
                		last_screen = screen;
                		screen = 200;// Calibration
                		break;
					} else if(buttons & 0x04)
					{
                		HAL_Delay(debounceDelay);
                		buttons = 0;
                		last_screen = screen;
                		screen = 300; // Settings
                		break;
					} else if(buttons & 0x08)
					{
                		HAL_Delay(debounceDelay);
                		buttons = 0;
                		last_screen = screen;
                		screen = 400; // Data
                		break;
					} else if(buttons & 0x10)
					{
                		HAL_Delay(debounceDelay);
                		buttons = 0;
                		last_screen = screen;
                		screen = 500; // Graphing
                		break;
					}
                }

                break;

            // ---------- TEST MENU ----------
            case 100:
                both = 0;
                displayText(screen, 0);

                char buf[32];
                snprintf(buf, 32, "Heater Lvl: %.1f W", heater==1?0.1:heater==2?0.3:heater==3?0.5:0);
                Displ_WString(480/2 - 11*strlen(buf)/2 + 10, 24, buf, Font16, 1, MAINTEXTCOLOUR, BACKGROUNDCOLOUR);
                snprintf(buf, 32, "Time: %d s", deltaTime);
                Displ_WString(480/2 - 11*strlen(buf)/2 + 10, 24+16, buf, Font16, 1, MAINTEXTCOLOUR, BACKGROUNDCOLOUR);
                snprintf(buf, 32, "Delta Temp: %.0f C", deltaTemp);
                Displ_WString(480/2 - 11*strlen(buf)/2 + 10, 24+16*2, buf, Font16, 1, MAINTEXTCOLOUR, BACKGROUNDCOLOUR);

                button = read_buttons();
                switch(button) {
                    case 1: // Default
//                        deltaTemp = deltaTempDefault;
//                        deltaTime = deltaTimeDefault;
//                        heater = heaterDefault;
                        last_screen = screen;
                        screen = 120;
                        break;
                    case 2: last_screen = screen; screen = 110; break; // Temp select
                    case 3: last_screen = screen; both = 1; screen = 110; break; // Both
                    case 4: last_screen = screen; screen = 110; break; // Time select
                    case 5: last_screen = screen; screen = 115; break; //heater selection
                    case 6: last_screen = screen; screen = 99; break; // Back to main
                    default: break;
                }
                break;


            case 110: // Temp
                displayText(screen, 0);

                snprintf(buf, 32, "Max Temp: ");
                Displ_WString(20, 320/4, buf, Font16, 1, MAINTEXTCOLOUR, BACKGROUNDCOLOUR);
                ii = (int)deltaTemp;
                selectNumber(&ii, 3, 20+11*10, 320/4);
                deltaTemp = (double)ii;

                if (both == 1){
                	 last_screen = screen;
                	screen = 111; break;
                }
                last_screen = screen;
                screen = 100;
                break;

            case 111: // Time
                displayText(screen, 0);
                snprintf(buf, 32, "Max Time: ");
                Displ_WString(20, 320/4, buf, Font16, 1, MAINTEXTCOLOUR, BACKGROUNDCOLOUR);
                ii = (int)deltaTime;
                selectNumber(&ii, 4, 20+11*10, 320/4);
                deltaTime = ii;
                last_screen = screen;
                screen = 100;
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
            	last_screen = screen;
            	screen = 100;
            	break;

            case 120: // Test running
                displayText(screen, 0);
                runTest(deltaTime, deltaTemp, heater);
                last_screen = screen;
                screen = 130;
                break;

            case 130: // Test finished
                displayText(screen, 0);
                Displ_WString(480/2 - 11*strlen(filename)/2, 320-10-16, filename, Font16, 1, SECONDARYTEXTCOLOUR, BACKGROUNDCOLOUR);

                button = read_buttons();
                if (button == 1){
                	last_screen = screen;
                	screen = 501;
                	break;
                }
                else if (button == 2){
                	sd_delete_file(filename);
                }
                last_screen = screen;
                screen = 99; // Return to main menu
                break;

            // ---------- CALIBRATION MENU----------
            case 200:
                displayText(screen, 0);
                button = read_buttons();
                switch(button) {
                    case 1: last_screen = screen; screen = 210; break;
                    case 2: /* set default calibration */ break;
                    case 6: last_screen = screen; screen = 99; break;
                }
                break;

            case 210: // Material select
                displayText(screen, 0);
                button = read_buttons();
                switch(button) {
                    case 1: runCalibration(1); break; /* Material A calibration */
                    case 2: runCalibration(2); break; /* Material B calibration */
                    case 6: last_screen = screen; screen = 200; break;
                }
                break;

            case 220: // Cancel (future)
                displayText(screen, 0);
                if (read_buttons() == 6)
                {
                	last_screen = screen;
                	screen = 210;
                }
                break;

            case 230: // Return
                displayText(screen, 0);
                if (read_buttons())
                {
                	last_screen = screen;
                	screen = 200;
                }
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
                    case 5: if(getVerification())
                    {
                    	Flash_WriteFileID(0);
                    }
                    break;
                    case 6: screen = 99; break;
                }
                break;

//            case 310: // Brightness
//                displayText(screen, 0);
//                brightness = selectNumber('B', 0);
//                last_screen = screen;
//                screen = 300;
//                break;

            case 320: // Default temperature
                displayText(screen, 0);
//                deltaTempDefault = selectNumber('E', 0);
                snprintf(buf, 32, "Max Temp: ");
                Displ_WString(20, 320/4, buf, Font16, 1, MAINTEXTCOLOUR, BACKGROUNDCOLOUR);
                ii = (int)deltaTemp;
                selectNumber(&ii, 4, 20+11*10, 320/4);
                deltaTemp = (double)ii;

                last_screen = screen;
                screen = 300;
                break;

            case 330: // Default duration
                displayText(screen, 0);
//                deltaTimeDefault = selectNumber('I', 0);
                snprintf(buf, 32, "Max Time: ");
                Displ_WString(20, 320/4, buf, Font16, 1, MAINTEXTCOLOUR, BACKGROUNDCOLOUR);
                ii = (int)deltaTime;
                selectNumber(&ii, 4, 20+11*10, 320/4);
                deltaTime = (double)ii;

                last_screen = screen;
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
            	last_screen = screen;
            	screen = 300;
            	break;

            // ---------- OTHER ----------
            case 400:
                displayText(screen, 0);
                // Future development?
                button = read_buttons();
                switch(button) {
                    case 6: last_screen = screen; screen = 99; break;
                }
                break;

            // --------- Load old results. ------
			case 500:
				int s = selectFile();
				switch(s)
				{
				case 1:
					last_screen = screen;
					screen = 501;
					break;
				case 2:
					last_screen = screen;
					screen = 401;
					break;
				default:
					last_screen = screen;
					screen = 99;
					break;
				}
				break;

			case 501:
				displayText(screen, 0);
				showGraphWithMarkers(40, 80, 360, 215);
				if(last_screen == 500)
				{
					last_screen = screen;
					screen = 500;
				}
				else if(last_screen == 130)
				{
					last_screen = screen;
					screen = 130;
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
