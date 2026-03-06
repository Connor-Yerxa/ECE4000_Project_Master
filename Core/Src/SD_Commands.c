#include "SD_Commands.h"

// Don't forget to free temps after use!
float * readMeasurementData(char * filename, int * tempsLen)
{
	FIL file;
	FRESULT fin = f_open(&file, filename, FA_READ);
	if(fin != FR_OK) return NULL;

	float * temps = NULL;
	*tempsLen = 0;

	char line[10];

	uint8_t linesToTemps=20;
	for(int i=0;i<linesToTemps;i++) f_gets((TCHAR*)line, 10, &file);

	while(f_gets((TCHAR*)line, 10, &file) != 0 || *temps < 100)
	{
		float newTemp = atof(line);

		float * newTemps = realloc(temps, sizeof(float) * ++(*tempsLen));
		if(newTemps == NULL)
		{
			f_close(&file);
			return NULL;
		}
		temps = newTemps;
		temps[*tempsLen - 1] = newTemp;
	}

	f_close(&file);
	return temps;
}

int WriteMetaData(FIL fil, char * metaData, UINT bytesToWrite)
{
	/*
	#,Test ID:,
	#,Instrument:,
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
	return 0;
}

int createMeasurementFile(char * filename, float temp)
{

}
