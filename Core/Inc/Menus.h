/*
 * Menus.h
 *
 *  Created on: Mar 10, 2026
 *      Author: nateh
 */

#ifndef INC_MENUS_H_
#define INC_MENUS_H_

#include <stdio.h>

// ---------- Global variables ----------
extern int screen;           // Current screen
extern int button;           // Last button pressed
extern int number;           // Number input from numberSelect
extern double deltaTemp;     // Current temperature for test
extern double deltaTime;     // Current time for test
extern double deltaTempDefault;  // Default temperature
extern double deltaTimeDefault;  // Default duration
extern int brightness;       // Display brightness
extern int both;             // Flag for "both" option in test
extern int heater;
extern float calCoef;

// ---------- Functions ----------
void menus();                // Main menu loop


#endif /* INC_MENUS_H_ */
