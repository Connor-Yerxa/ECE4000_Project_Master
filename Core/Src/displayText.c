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
void displayText(int screenID, int option) {

    printf("Displaying screen %d with option %d\n", screenID, option); //TEST
 //   clearScreen(0xFFFF);
    UI_DrawLine(0,"Screen:             ",   screenID);
    //UI_DrawLine()

    if (option == 0){

    }
    else if(option ==1){

    }

    //display output on the actual screen


}
