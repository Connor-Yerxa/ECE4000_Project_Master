#ifndef __SD_COMMANDS_H
#define __SD_COMMANDS_H

#ifdef __cplusplus
	extern "C" {
#endif

#include "stdio.h"
#include "stdlib.h"
#include "main.h"
#include "sd_functions.h"


float * readMeasurementData(char * filename, int * tempsLen);
int WriteMetaData(FIL fil, char * metaData, UINT bytesToWrite);
int createMeasurementFile(char * filename, float temp);

#ifdef __cplusplus
	}
#endif

#endif /* __SD_COMMANDS_H */
