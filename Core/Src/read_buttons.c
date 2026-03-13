/*
 * read_buttons.c
 *
 *  Created on: Mar 10, 2026
 *      Author: nateh
 */


// read_Buttons.c
//#include "main.h"
#include "read_Buttons.h"
#include "main.h"

uint32_t debounce_delay = 500;

// External global variable, set elsewhere (e.g., via interrupt or input polling)
//extern volatile int buttons;

int read_buttons(void) {
    int pressed = buttons;

    // Wait until buttons is non-zero
    while (pressed == 0) {
        pressed = buttons;
    }

    // Reset buttons to 0 after reading
    switch(pressed) { //convert pressed to 1-6
        case 1:
        	HAL_Delay(debounce_delay);
            pressed = 1;
            buttons=0;
            break;
        case 2:
        	HAL_Delay(debounce_delay);
            pressed = 2;
            buttons=0;
            break;
        case 4:
        	HAL_Delay(debounce_delay);
            pressed = 3;
            buttons=0;
            break;
        case 8:
        	HAL_Delay(debounce_delay);
            pressed = 4;
            buttons=0;
            break;
        case 16:
        	HAL_Delay(debounce_delay);
            pressed = 5;
            buttons=0;
            break;
        case 32:
        	HAL_Delay(debounce_delay);
            pressed = 6;
            buttons=0;
            break;
        default:
            break;
    }
    return pressed;
}
