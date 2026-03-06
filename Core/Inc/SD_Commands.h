#ifndef __SD_COMMANDS_H
#define __SD_COMMANDS_H

#ifdef __cplusplus
	extern "C" {
#endif

#include "stdio.h"
#include "stdlib.h"
#include "main.h"
#include "sd_functions.h"
#include <string.h>

typedef struct {
	float power;
	int testID; //100
	char * project; //"ENGG4000-B"
	char * material; //"N/A"
	char * sampleID; //"N/A"
	float regionStart; //0
	float regionEnd; //0
	float conductivity; //0
	float R_Squared; //0
	uint8_t calibrationApplied; //0
}METADATA;

void readMeasurementData(char * filename, int * tempsLen, int maxprintout);
uint8_t WriteMetaData(char * filename, METADATA md);
uint8_t createMeasurementFile(char ** filename,  METADATA * md);

#ifdef __cplusplus
	}
#endif

#endif /* __SD_COMMANDS_H */
