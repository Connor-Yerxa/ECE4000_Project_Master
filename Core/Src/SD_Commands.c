#include "SD_Commands.h"

// Don't forget to free temps after use!
void readMeasurementData(char * filename, int * tempsLen, int maxprintout)
{
	FIL file;
	FRESULT fin = f_open(&file, filename, FA_READ);
	if(fin != FR_OK) printf("Couln\'t open: %s", filename);

	*tempsLen = 0;

	char line[64];

	uint8_t linesToTemps=20;
	for(int i=0;i<linesToTemps;i++)
	{
		f_gets((TCHAR*)line, 64, &file);
	}

	int i=0;
	float newTemp;
	while(f_gets((TCHAR*)line, 10, &file) != 0 && (i < maxprintout || maxprintout == 0))
	{
		newTemp = atof(line);

		printf("Time %.2f: %.3f C\n", (float)i/15.0, newTemp);
		i++;
	}

	f_close(&file);
}

uint8_t WriteMetaData(char * filename, METADATA md)
{
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

	sd_append_file(filename, "#,Sensor Serial:,          \n");
	sd_append_file(filename, "#,Test ID:,          \n");
	sd_append_file(filename, "#,Instrument:,TLS Handheld\n");
	sd_append_file(filename, "#,Project:,          \n");
	sd_append_file(filename, "#,Material:,          \n");
	sd_append_file(filename, "#,Sensor Serial:,          \n");
	sd_append_file(filename, "#,Sample ID:,          \n\n\n");

	sd_append_file(filename, "#,Power Level:,          \n\n\n");

	sd_append_file(filename, "#,Region Start:,          \n");
	sd_append_file(filename, "#,Region End:,          \n");
	sd_append_file(filename, "#,Material:,          \n");
	sd_append_file(filename, "#,Conductivity:,          \n");
	sd_append_file(filename, "#,Calibration Applied:, \n\n\n");

	sd_append_file(filename, "Delta Temperature (degC)\n");


	return 0;
}

uint8_t createMeasurementFile(char * filename,  METADATA * md)
{
	sd_write_file(filename, "");

	FILINFO info;

	uint8_t day   =  md->gps.utc_date / 10000;          	// DD
	uint8_t month = (md->gps.utc_date / 100) % 100;     	// MM
	uint8_t year  =  md->gps.utc_date % 100;            	// YY


	uint8_t hr   =  md->gps.utc_date / 10000;          	// hr
	uint8_t min = (md->gps.utc_date / 100) % 100;		// mm
	uint8_t sec  =  md->gps.utc_date % 100;            	// ss

	info.fdate = ((year - 1980) << 9) | (month << 5) | day;
	info.ftime = (hr << 11) | (min << 5) | (sec / 2);

	f_utime(filename, &info);

	if(WriteMetaData(filename, *md))
	{
		return 1;
	}

	return 0;
}
