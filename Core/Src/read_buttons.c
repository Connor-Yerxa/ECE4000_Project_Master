/*
 * read_buttons.c
 *
 *  Created on: Mar 10, 2026
 *      Author: nateh
 */


// read_Buttons.c
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
    buttons = 0;

    return pressed;
}
