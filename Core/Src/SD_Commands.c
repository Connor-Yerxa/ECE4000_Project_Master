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

	while(f_gets((TCHAR*)line, 10, &file) != 0)
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

int writeMeasurements(FIL fil, char * filename, float * temps, int tempsLen, char * metaData, UINT bytesToWrite)
{
	FRESULT fout = f_open(&fil, filename, FA_WRITE | FA_OPEN_ALWAYS | FA_CREATE_ALWAYS);
	if(fout != FR_OK) return -1;

	if(WriteMetaData(fil, metaData, bytesToWrite) != 0)
	{
		f_close(&fil);
		return -1;
	}

	for(int i=0;i<tempsLen;i++)
	{
		BYTE t[5];
		UINT written;
		sprintf((char*)t, "%.2f", temps[i]);
		f_write(&fil, t, 4, &written);
	}

	f_close(&fil);
}
