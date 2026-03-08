/*
 * MAX_Commands.h
 *
 *  Created on: Mar 6, 2026
 *      Author: cbyer
 */

#ifndef INC_MAX_COMMANDS_H_
#define INC_MAX_COMMANDS_H_

#include "max31856_stm32.h"
#include "max31865_stm32.h"

#define USE_MAX_RTD 0

extern MAX31865_HandleTypeDef RTD_MAX;
extern max31856_t THERM_MAX;

void MAX_INITs(SPI_HandleTypeDef * hspi);
float readTemp();

#endif /* INC_MAX_COMMANDS_H_ */
