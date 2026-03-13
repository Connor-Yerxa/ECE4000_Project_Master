#include "SD_Commands.h"
#include "main.h"
#include "runCalibration.h"
#include "Menus.h"

#define USE_LINEAR_REGRESSION 0


const char * const MetadataLabelStrings[META_LABEL_COUNT] = {
    [META_SENSOR_SERIAL]       = "#,Sensor Serial:,",
    [META_TEST_ID]             = "#,Test ID:,",
    [META_INSTRUMENT]          = "#,Instrument:,",
    [META_PROJECT]             = "#,Project:,",
    [META_MATERIAL]            = "#,Material:,",
    [META_SAMPLE_ID]           = "#,Sample ID:,",
    [META_LATITUDE]            = "#,Latitude:,",
    [META_LONGITUDE]           = "#,Longitude:,",
    [META_POWER]               = "#,Power:,",
    [META_REGION_START]        = "#,Region Start:,",
    [META_REGION_END]          = "#,Region End:,",
    [META_CONDUCTIVITY]        = "#,Conductivity:,",
    [META_CALIBRATION_APPLIED] = "#,Calibration Applied:,"
};


// Don't forget to free temps after use!
void readMeasurementData(int * tempsLen, int maxprintout) //broken, needs adjusting for time added.
{
	FIL file;
	FRESULT fin = f_open(&file, filename, FA_READ);
	if(fin != FR_OK) printf("Couln\'t open: %s", filename);

	*tempsLen = 0;

	char line[64];

//	uint8_t linesToTemps=20;
	while(f_gets((TCHAR*)line, 64, &file) && !strstr(line, "Delta Temperature (degC)"));

	int i=0;
	float newTemp;
	while(f_gets((TCHAR*)line, 64, &file) != 0 && (i < maxprintout || maxprintout == 0))
	{
		newTemp = atof(line);

		printf("Time %.2f: %.3f C\n", (float)i/15.0, newTemp);
		i++;
	}

	f_close(&file);
}

//uint8_t updateMetaData(char * filename, MetadataLabel fieldLabel, char * newValue)
//{
//	const char * fl = MetadataLabelStrings[fieldLabel];
//	FIL file;
//	FRESULT fin = f_open(&file, filename, FA_READ | FA_WRITE);
//	if(fin != FR_OK) printf("Couln\'t open: %s", filename);
//
//	char line[64];
//	DWORD lineStart = f_tell(&file);
//	while(f_gets((TCHAR*)line, 64, &file) != 0 && strstr(line, (char*)fl) == NULL)
//	{
//		lineStart = f_tell(&file);
//	}
//
//	size_t originalLen = strlen(line);
//	char newLine[64];
//	char spaces[space_count + 1]; //max_spacees + '\0'
//	int i;
//	for(i=0;i<(originalLen-strlen(fl)-strlen(newValue));i++)
//	{
//		spaces[i] = ' ';
//	}
//	spaces[i] = '\0';
//
//	sprintf(newLine, "%s%s%s\n", fl, newValue, spaces);
//
//	UINT bw;
//	f_lseek(&file, lineStart);
//	f_write(&file, newLine, originalLen, &bw);
//
//
//	f_close(&file);
//	return 0;
//}

uint8_t updateMetaData(char * filename, MetadataLabel fieldLabel, char * newValue)
{
    const char * fl = MetadataLabelStrings[fieldLabel];

    FIL file;
    if (f_open(&file, filename, FA_READ | FA_WRITE) != FR_OK)
        return 1;

    char line[128];
    DWORD lineStart = 0;

    while (1)
    {
        lineStart = f_tell(&file);

        if (!f_gets(line, sizeof(line), &file))
            break; // EOF

        if (strstr(line, fl))
            break; // found
    }

    // Not found
    if (!strstr(line, fl)) {
        f_close(&file);
        return 2;
    }

    size_t originalLen = strlen(line);

    // Remove newline(s)
    while (originalLen > 0 && (line[originalLen-1] == '\n' || line[originalLen-1] == '\r'))
        originalLen--;

    size_t flLen = strlen(fl);
    size_t newLen = strlen(newValue);

    int spaceCount = originalLen - flLen - newLen;
    if (spaceCount < 0) spaceCount = 0;

    char newLine[128];
    int pos = 0;

    memcpy(newLine + pos, fl, flLen);
    pos += flLen;

    memcpy(newLine + pos, newValue, newLen);
    pos += newLen;

    memset(newLine + pos, ' ', spaceCount);
    pos += spaceCount;

    // Restore newline
    newLine[pos++] = '\n';

    // Write back
    f_lseek(&file, lineStart);
    UINT bw;
    f_write(&file, newLine, pos, &bw);

    FILINFO fno;
    fno.fdate = (gps_data.full_timestamp >> 16) & 0xFFFF;
    fno.ftime = gps_data.full_timestamp & 0xFFFF;
    f_utime(filename, &fno);

    f_close(&file);
    return 0;
}

uint8_t WriteMetaData(char * filename, METADATA md)
{
	char field[64];
	/*
	#,Sensor Serial:,
	#,Test ID:,
	#,Instrument:,TLS Handheld
	#,Project:,
	#,Material:,
	#,Sample ID:,

	#,Power Level:,

	#,Region Start:,
	#,Region End:,
	#,Conductivity:,
	#,R Squared:,
	#,Calibration Applied:,
	 */

	sprintf(field, "%s%s\n", MetadataLabelStrings[META_SENSOR_SERIAL], META_SPACE);
	sd_append_file(filename, field);
	sprintf(field, "%s%s\n", MetadataLabelStrings[META_TEST_ID], META_SPACE);
	sd_append_file(filename, field);
	sprintf(field, "%s%s\n", MetadataLabelStrings[META_INSTRUMENT], META_SPACE);
	sd_append_file(filename, field);
	sprintf(field, "%s%s\n", MetadataLabelStrings[META_PROJECT], META_SPACE);
	sd_append_file(filename, field);
	sprintf(field, "%s%s\n", MetadataLabelStrings[META_MATERIAL], META_SPACE);
	sd_append_file(filename, field);
	sprintf(field, "%s%s\n", MetadataLabelStrings[META_SAMPLE_ID], META_SPACE);
	sd_append_file(filename, field);

	sprintf(field, "%s%s\n", MetadataLabelStrings[META_LATITUDE], META_SPACE);
	sd_append_file(filename, field);
	sprintf(field, "%s%s\n", MetadataLabelStrings[META_LONGITUDE], META_SPACE);
	sd_append_file(filename, field);

	sprintf(field, "%s%s\n\n\n", MetadataLabelStrings[META_POWER], META_SPACE);
	sd_append_file(filename, field);

	sprintf(field, "%s%s\n", MetadataLabelStrings[META_REGION_START], META_SPACE);
	sd_append_file(filename, field);
	sprintf(field, "%s%s\n", MetadataLabelStrings[META_REGION_END], META_SPACE);
	sd_append_file(filename, field);
	sprintf(field, "%s%s\n", MetadataLabelStrings[META_CONDUCTIVITY], META_SPACE);
	sd_append_file(filename, field);
	sprintf(field, "%s%s\n\n\n", MetadataLabelStrings[META_CALIBRATION_APPLIED], META_SPACE);
	sd_append_file(filename, field);

	sd_append_file(filename, "Time (s),lnTime (s),Delta Temperature (degC)\n");


	return 0;
}

uint8_t createMeasurementFile(METADATA * md)
{
	char filenameSnipped[28];
	strcpy(filenameSnipped, filename);

	char *dot = strtok(filenameSnipped, ".");
	strcpy(filenameSnipped, dot);

	printf("snipped: %s\n", filenameSnipped);

	uint8_t i=1;

	FIL file;
	char newfilename[32];
	snprintf(newfilename, 32, "%s.csv", filenameSnipped);
	FRESULT res = f_open(&file, newfilename, FA_READ);
	while(res == FR_OK)
	{
		f_close(&file);
		sprintf(newfilename, "%s%d.csv", filenameSnipped, i++);
		res = f_open(&file, newfilename, FA_READ);
	}
	f_close(&file);
	strcpy(filename, newfilename);

	GPS_oneshot();

	sd_write_file(newfilename, "");

	if(WriteMetaData(newfilename, *md))
	{
		return 1;
	}

	return 0;
}

uint8_t appendTemp(char * filename, float delta_temp, uint32_t delta_time)
{
	char line[64];
	float t = (float)delta_time / 1000;
	float lnt = logf(t);
	sprintf(line, "%.4f,%.4f,%.4f\n", t, lnt, delta_temp);
	FRESULT res = sd_append_file(filename, line);
	return (uint8_t)res;
}


float calculateK(float startTime, float stopTime, char* filename, float power){ //pick start & stoptimes
	FIL file;
	FRESULT fin = f_open(&file, filename, FA_READ);
	if(fin != FR_OK) printf("Couln\'t open: %s", filename);

	char line[64];
	char * token;

	while(f_gets((TCHAR*)line, 64, &file) && !strstr(line, "Delta Temperature (degC)"));
	while(f_gets((TCHAR*)line, 64, &file) != 0)
	{
		token = strtok(line, ",");
		float currentTime = atof(token);
		if(currentTime >= startTime) break;
	}
	token = strtok(NULL, ",");
	float lnstarttime = atof(token);
	token = strtok(NULL, ",");
	float starttemp = atof(token);


	while(f_gets((TCHAR*)line, 64, &file) != 0)
	{
		token = strtok(line, ",");
		float currentTime = atof(token);
		if(currentTime >= startTime) break;
	}
	token = strtok(NULL, ",");
	float lnstoptime = atof(token);
	token = strtok(NULL, ",");
	float stoptemp = atof(token);

	float slope = (stoptemp - starttemp) / (lnstoptime - lnstarttime);

	float k;
	if(USE_LINEAR_REGRESSION)
	{
		// Linear regression code
		k = 0;
	}else
	{
		k = power / (4 * M_PI * slope *0.15) * calCoef;
	}

	f_close(&file);
	return k;
}

char * getMetaData(char * filename, MetadataLabel label){
	FIL file;
	FRESULT fin = f_open(&file, filename, FA_READ);
	if(fin != FR_OK) printf("Couln\'t open: %s", filename);

	char line[64];

	while(f_gets((TCHAR*)line, 64, &file) != 0 && strstr(line, (char*)label) == NULL);

	char * token = strtok(line, ",");
	token = strtok(NULL, ",");
	token = strtok(NULL, ",");

	char * buf = strdup(token);

	f_close(&file);
	return buf;
}
