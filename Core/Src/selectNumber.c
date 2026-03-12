/*
 * selectNumber.c
 *
 *  Created on: Mar 10, 2026
 *      Author: nateh
 */
#include <stdio.h>
#include "read_buttons.h"
#include "displayText.h"
#include <math.h>


int power10(int n){
    int result = 1;
    for(int i = 0; i < n; i++)
        result *= 10;
    return result;
}

int selectNumber(char Type, int a) {

	int base = 0;
	int tempValue = 0;
	int numbersToReturn;
	int value = 0;
	int totalValue = 0;
	int pressed = 0;
	int ntrn = 0;

    if (Type == 'E') {        // temperature
        numbersToReturn = 3;
        base = 10;
    }
    else if (Type == 'I') {   // time (IN SECONDS)
        numbersToReturn = 3;
        base = 10;
    }
    else if (Type == 'B') {   // brightness
        numbersToReturn = 1;
        base = 10;
    }
    else {
        return 0;
    }

    while (numbersToReturn > 0) {

    	ntrn = numbersToReturn - 1;
        pressed = read_buttons();

        if (pressed == 1) {         // increase digit
            value++;
            if (value >= base){
                value = 0;
            }
            tempValue = value * power10(ntrn);
            if (tempValue > 200){
            	value = 1;
            	tempValue = value * power10(ntrn);
            }
            tempValue = tempValue + (totalValue * power10(numbersToReturn));
            displayText(tempValue, 1);
        }
        else if (pressed == 3) {         // decrease digit
        	value--;
            if (0 > value){
                value = base - 1;
            }
            tempValue = value* power10(ntrn);
            if (tempValue > 200){
            	value = 1;
            	tempValue = value * power10(ntrn);
            }
            tempValue = tempValue + (totalValue * power10(numbersToReturn));
            displayText(tempValue, 1);
        }
        else if (pressed == 2) {         // confirm digit
            totalValue = totalValue * 10 + value;
            tempValue = (totalValue*power10(ntrn));
            displayText(tempValue, 1);
            value = 0;
            numbersToReturn--;
        }


    }

    // final limits
    if (Type == 'B' && totalValue > 9){
        totalValue = 9;
	}
    if (Type == 'I' && totalValue > 180){
        totalValue = 180;
	}
    if (Type == 'E' && totalValue > 100){
    	totalValue = 100;
	}

    return totalValue;
}
