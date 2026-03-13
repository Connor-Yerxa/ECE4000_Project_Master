/*
 * read_buttons.c
 *
 *  Created on: Mar 10, 2026
 *      Author: nateh
 */


// read_Buttons.c
//#include "main.h"
#include "read_Buttons.h"

// External global variable, set elsewhere (e.g., via interrupt or input polling)
extern volatile int buttons;

int read_buttons(void) {
    int pressed = 0;

    // Wait until buttons is non-zero
    while (pressed == 0) {
        pressed = buttons;
    }

    // Reset buttons to 0 after reading
    switch(pressed) { //convert pressed to 1-6
        case 1:
        	HAL_Delay(100);
            pressed = 1;
            break;
        case 2:
        	HAL_Delay(100);
            pressed = 2;
            break;
        case 4:
        	HAL_Delay(100);
            pressed = 3;
            break;
        case 8:
        	HAL_Delay(100);
            pressed = 4;
            break;
        case 16:
        	HAL_Delay(100);
            pressed = 5;
            break;
        case 32:
        	HAL_Delay(100);
            pressed = 6;
            break;
        default:
            break;
    }
    buttons = 0;
    return pressed;
}
