/*
 * selectNumber.c
 *
 *  Created on: Mar 10, 2026
 *      Author: nateh
 */
#include <stdio.h>
#include <stdio.h>
#include "read_buttons.h"
#include "displayText.h"
#include <math.h>
#include "main.h"
#include "menus.h"


//int power10(int n){
//    int result = 1;
//    for(int i = 0; i < n; i++)
//        result *= 10;
//    return result;
//}
//
//int selectNumber(char Type, int a) {
//
//	int base = 0;
//	int tempValue = 0;
//	int numbersToReturn;
//	int value = 0;
//	int totalValue = 0;
//	int pressed = 0;
//	int ntrn = 0;
//
//	char buf[16];
//
//    if (Type == 'E') {        // temperature
//        numbersToReturn = 3;
//        base = 10;
//    }
//    else if (Type == 'I') {   // time (IN SECONDS)
//        numbersToReturn = 3;
//        base = 10;
//    }
//    else if (Type == 'B') {   // brightness
//        numbersToReturn = 1;
//        base = 10;
//    }
//    else {
//        return 0;
//    }
//
//    while (numbersToReturn > 0) {
//
//    	ntrn = numbersToReturn - 1;
//        pressed = read_buttons();
//
//        if (pressed == 1) {         // increase digit
//            value++;
//            if (value >= base){
//                value = 0;
//            }
//            tempValue = value * power10(ntrn);
//            if (tempValue > 200){
//            	value = 1;
//            	tempValue = value * power10(ntrn);
//            }
//            tempValue = tempValue + (totalValue * power10(numbersToReturn));
//            displayText(tempValue, 1);
//            snprintf(buf, 16, "%d", tempValue);
//            Displ_WString(5, 320/2-16/2, buf, Font16, 1, MAINTEXTCOLOUR, BACKGROUNDCOLOUR);
//        }
//        else if (pressed == 3) {         // decrease digit
//        	value--;
//            if (0 > value){
//                value = base - 1;
//            }
//            tempValue = value* power10(ntrn);
//            if (tempValue > 200){
//            	value = 1;
//            	tempValue = value * power10(ntrn);
//            }
//            tempValue = tempValue + (totalValue * power10(numbersToReturn));
//            displayText(tempValue, 1);
//        }
//        else if (pressed == 2) {         // confirm digit
//            totalValue = totalValue * 10 + value;
//            tempValue = (totalValue*power10(ntrn));
//            displayText(tempValue, 1);
//            value = 0;
//            numbersToReturn--;
//        }
//
//
//    }
//
//    // final limits
//    if (Type == 'B' && totalValue > 9){
//        totalValue = 9;
//	}
//    if (Type == 'I' && totalValue > 180){
//        totalValue = 180;
//	}
//    if (Type == 'E' && totalValue > 100){
//    	totalValue = 100;
//	}
//
//    return totalValue;
//}

void selectNumber(int * value, int nDigits, int xoffset, int yoffset)
{
	int newValue = *value;
	int pos=1;

	char buf[32];
	snprintf(buf, 32, "%4d", *value);
	Displ_WString(xoffset, yoffset, buf, Font16, 1, MAINTEXTCOLOUR, BACKGROUNDCOLOUR);

	Displ_WString(xoffset+11*(4-log10(pos)-1), yoffset-16, "V", Font16, 1, SECONDARYTEXTCOLOUR, BACKGROUNDCOLOUR);

	while(1)
	{
		printf("%d\n", newValue);
		if(pos > pow(10, nDigits-2))
		{
			Displ_WString(xoffset+11*(4-log10(pos)-1), yoffset-16, "V", Font16, 1, BACKGROUNDCOLOUR, BACKGROUNDCOLOUR);
			pos = 1;
			Displ_WString(xoffset+11*(4-log10(pos)-1), yoffset-16, "V", Font16, 1, SECONDARYTEXTCOLOUR, BACKGROUNDCOLOUR);
		}
		if(pos < 1)
		{
			Displ_WString(xoffset+11*(4-log10(pos)-1), yoffset-16, "V", Font16, 1, BACKGROUNDCOLOUR, BACKGROUNDCOLOUR);
			pos = pow(10, nDigits-2);
			Displ_WString(xoffset+11*(4-log10(pos)-1), yoffset-16, "V", Font16, 1, SECONDARYTEXTCOLOUR, BACKGROUNDCOLOUR);
		}

		if(buttons & 0x01) //up
		{
			HAL_Delay(debounceDelay);
			buttons = 0;

			Displ_WString(xoffset, yoffset, buf, Font16, 1, BACKGROUNDCOLOUR, BACKGROUNDCOLOUR);
			newValue += pos;
			snprintf(buf, 32, "%4d", newValue);
			Displ_WString(xoffset, yoffset, buf, Font16, 1, MAINTEXTCOLOUR, BACKGROUNDCOLOUR);

		}else if(buttons & 0x02) //prev
		{
			HAL_Delay(debounceDelay);
			buttons = 0;

			Displ_WString(xoffset+11*(4-log10(pos)-1), yoffset-16, "V", Font16, 1, BACKGROUNDCOLOUR, BACKGROUNDCOLOUR);
			pos *= 10;
			Displ_WString(xoffset+11*(4-log10(pos)-1), yoffset-16, "V", Font16, 1, SECONDARYTEXTCOLOUR, BACKGROUNDCOLOUR);

		}else if(buttons & 0x04) //down
		{
			HAL_Delay(debounceDelay);
			buttons = 0;

			Displ_WString(xoffset, yoffset, buf, Font16, 1, BACKGROUNDCOLOUR, BACKGROUNDCOLOUR);
			newValue -= pos;
			snprintf(buf, 32, "%4d", newValue);
			Displ_WString(xoffset, yoffset, buf, Font16, 1, MAINTEXTCOLOUR, BACKGROUNDCOLOUR);

		}else if(buttons & 0x08) //save
		{
			HAL_Delay(debounceDelay);
			buttons = 0;
			*value = newValue;
			return;

		}else if(buttons & 0x10) //next
		{
			HAL_Delay(debounceDelay);
			buttons = 0;

			Displ_WString(xoffset+11*(4-log10(pos)-1), yoffset-16, "V", Font16, 1, BACKGROUNDCOLOUR, BACKGROUNDCOLOUR);
			pos /= 10;
			Displ_WString(xoffset+11*(4-log10(pos)-1), yoffset-16, "V", Font16, 1, SECONDARYTEXTCOLOUR, BACKGROUNDCOLOUR);

		}else if(buttons & 0x20) //back
		{
			HAL_Delay(debounceDelay);
			buttons = 0;
			return;
		}
	}
}
