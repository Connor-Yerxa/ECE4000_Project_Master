/*
 * MAX_Commands.c
 *
 *  Created on: Mar 6, 2026
 *      Author: cbyer
 */
#include "MAX_Commands.h"
#include "max31856_stm32.h"
#include "max31865_stm32.h"
#include "main.h"

const SPI_HandleTypeDef * hspi_blank;

MAX31865_HandleTypeDef RTD_MAX;
max31856_t THERM_MAX;

void MAX_INITs(SPI_HandleTypeDef * hspi)
{
	max31856_t newTHERM_MAX = {hspi, {THERM_CS_GPIO_Port, THERM_CS_Pin}};
	THERM_MAX = newTHERM_MAX;
	max31856_init(&THERM_MAX);
	max31856_set_thermocouple_type(&THERM_MAX, CR1_TC_TYPE_K);
	max31856_set_conversion_mode(&THERM_MAX, CR0_CONV_CONTINUOUS);
	max31856_set_noise_filter(&THERM_MAX, CR0_FILTER_OUT_60Hz);

	if(USE_MAX_RTD)
	{
		MAX31865_Init(&RTD_MAX, hspi, RTD_CS_GPIO_Port, RTD_CS_Pin, MAX31865_WIRES_3, 0);
		max31856_set_cold_junction_enable(&THERM_MAX, CR0_CJ_DISABLED);
		uint8_t buf[2] = {0, 0};
		max31856_write_nregisters(&THERM_MAX, MAX31856_CJTH, buf, 2);
	}
	else
	{
		max31856_set_cold_junction_enable(&THERM_MAX, CR0_CJ_ENABLED);
	}
}

float readTemp()
{
	if(USE_MAX_RTD)
	{
		uint16_t raw15;
		MAX31865_ReadRTDRaw(&RTD_MAX, &raw15);
		float cjTemp = MAX31865_Temperature_C(raw15, RTD_NOMINAL_OHMS, RREF_OHMS);

		cjTemp *= 256.0;\
		int16_t raw16 = cjTemp;
		uint8_t raw[2];
		raw[0] = (raw16 & 0xFF00) >> 8;
		raw[1] = raw16;

		max31856_write_nregisters(&THERM_MAX, MAX31856_CJTH, raw, 2);
	}

	float temp = max31856_read_TC_temp(&THERM_MAX);
	return temp;
}
