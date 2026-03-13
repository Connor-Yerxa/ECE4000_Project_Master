#ifndef __SD_COMMANDS_H
#define __SD_COMMANDS_H

#ifdef __cplusplus
	extern "C" {
#endif

#include "stdio.h"
#include "stdlib.h"
#include "sd_functions.h"
#include <string.h>

// Metadata field defines
typedef enum {
	META_SENSOR_SERIAL,
	META_TEST_ID,
	META_INSTRUMENT,
	META_PROJECT,
	META_MATERIAL,
	META_SAMPLE_ID,
	META_LATITUDE,
	META_LONGITUDE,
	META_POWER,
	META_REGION_START,
	META_REGION_END,
	META_CONDUCTIVITY,
	META_CALIBRATION_APPLIED,
	META_LABEL_COUNT   // keep last
} MetadataLabel;

#define META_SPACE "          "   // 10 spaces
#define space_count 10

extern const char * const MetadataLabelStrings[META_LABEL_COUNT];

//
//// 2. Map each enum to the exact label text in your file
//static const char * const MetadataLabelStrings[META_LABEL_COUNT] = {
//	[META_SENSOR_SERIAL]       = "#,Sensor Serial:,",
//	[META_TEST_ID]             = "#,Test ID:,",
//	[META_INSTRUMENT]          = "#,Instrument:,",
//	[META_PROJECT]             = "#,Project:,",
//	[META_MATERIAL]            = "#,Material:,",
//	[META_SAMPLE_ID]           = "#,Sample ID:,",
//	[META_LATITUDE]            = "#,Latitude:,",
//	[META_LONGITUDE]           = "#,Longitude:,",
//	[META_POWER]               = "#,Power:,",
//	[META_REGION_START]        = "#,Region Start:,",
//	[META_REGION_END]          = "#,Region End:,",
//	[META_CONDUCTIVITY]        = "#,Conductivity:,",
//	[META_CALIBRATION_APPLIED] = "#,Calibration Applied:,"
//};


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

void readMeasurementData(int * tempsLen, int maxprintout);
uint8_t updateMetaData(char * filename, MetadataLabel fieldLabel, char * newValue);
uint8_t WriteMetaData(char * filename, METADATA md);
uint8_t createMeasurementFile(METADATA * md);

uint8_t appendTemp(char * filename, float delta_temp, uint32_t delta_time);


float calculateK(float startTime, float stopTime, char* filename, float power);
char * getMetaData(char * filename, MetadataLabel label);

#ifdef __cplusplus
	}
#endif

#endif /* __SD_COMMANDS_H */
