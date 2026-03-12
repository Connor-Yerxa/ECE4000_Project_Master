/*
 * selectNumber.c
 *
 *  Created on: Mar 10, 2026
 *      Author: nateh
 */
#include <stdio.h>
#include "read_buttons.h"
#include "displayText.h"

int pressed;
int value;
int totalValue;
int numbersToReturn;
int base;
int tempValue;
int selectNumber(char Type, int a) {

    int base;
    int numbersToReturn;
    int value = 0;
    int totalValue = 0;
    int pressed;

    if (Type == 'E') {        // temperature
        numbersToReturn = 4;
        base = 10;
    }
    else if (Type == 'I') {   // time
        numbersToReturn = 3;
        base = 6;
    }
    else if (Type == 'B') {   // brightness
        numbersToReturn = 1;
        base = 10;
    }
    else if (Type == 'H') { //heater selection
    	numbersToReturn = 1;
    	base = 3;
    }
    else {
        return 0;
    }

    while (numbersToReturn > 0) {

        pressed = read_buttons();

        if (pressed == 1) {         // increase digit
            value++;
            tempValue = (totalValue * (10^numbersToReturn) + value * (10^(numbersToReturn - 1)));
            displayText(tempValue, 1);
            if (value >= base)
                value = 0;
        }
        else if (pressed == 3) {         // decrease digit
            if (value == 0){
                value = base - 1;
            }
            else{
                value--;
            }
            tempValue = (totalValue * (10^numbersToReturn) + value * (10^(numbersToReturn - 1)));
            displayText(tempValue, 1);
        }
        else if (pressed == 2) {         // confirm digit
            totalValue = totalValue * 10 + value;
            value = 0;
            tempValue = (totalValue * (10^numbersToReturn) + value * (10^(numbersToReturn - 1)));
            displayText(tempValue, 1);
            numbersToReturn--;
        }


    }

    // final limits
    if (Type == 'B' && totalValue > 9){
        totalValue = 9;
	}
    if (Type == 'I' && totalValue > 300){
        totalValue = 300;
	}
    if (Type == 'E' && totalValue > 1000){
    	totalValue = 1000;
	}

    return totalValue;
}
