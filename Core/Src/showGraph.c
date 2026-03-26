/*
 * showGraph.c
 *
 *  Created on: Mar 12, 2026
 *      Author: nateh
 */
#include "SD_Commands.h"
#include "main.h"
#include "displayText.h"
#include "menus.h"
#include "showGraph.h"
#include "Menus.h"

typedef struct {
    int16_t x1;          // pixel position of line 1
    int16_t x2;          // pixel position of line 2
    uint8_t active;      // 0 = line1, 1 = line2
} GraphMarkers;

int graph_yPoints[480];

int GRAPH_Y_MIN=0, GRAPH_Y_MAX=0;

float minLn, maxLn, minTemp, maxTemp;

float calculateK(float lnStart, float lnEnd, float calCoef, uint8_t useLinReg)
{
	float slope;
    // Ensure lnStart < lnEnd
    if (lnEnd < lnStart) {
        float tmp = lnStart;
        lnStart = lnEnd;
        lnEnd = tmp;
    }

    if(useLinReg)
    {
    	// Regression accumulators
		float sumX  = 0.0f;
		float sumY  = 0.0f;
		float sumXY = 0.0f;
		float sumX2 = 0.0f;
		int count   = 0;

		FIL file;
		char line[64];
		float t, ln, temp;

		if (f_open(&file, filename, FA_READ) != FR_OK) {
			printf("Failed to open file\n");
			return 0;
		}

		// Skip header
		while (f_gets(line, sizeof(line), &file) && !strstr(line, "Delta Temperature"));

		// Stream through file
		while (f_gets(line, sizeof(line), &file))
		{
			if (sscanf(line, "%f, %f, %f", &t, &ln, &temp) != 3)
				continue;

			printf("%f\n", ln);
			// Only include points inside the Ln range
			if (ln >= lnStart && ln <= lnEnd)
			{
				printf("Found\n");
				sumX  += ln;
				sumY  += temp;
				sumXY += ln * temp;
				sumX2 += ln * ln;
				count++;
			}

			if(ln >= lnEnd) break;
		}

		f_close(&file);

		if (count < 2) {
			printf("Not enough points for regression\n");
			return 0;
		}

		// Compute slope (m)
		float numerator   = (count * sumXY) - (sumX * sumY);
		float denominator = (count * sumX2) - (sumX * sumX);

		if (denominator == 0) {
			printf("Degenerate regression (vertical line)\n");
			return 0;
		}

		slope = numerator / denominator;
    }
    else
    {
    	FIL file;
		char line[64];
		float t, ln, temp, startln, endln, starttemp, endtemp;

		if (f_open(&file, filename, FA_READ) != FR_OK) {
			printf("Failed to open file\n");
			return 0;
		}

		// Skip header
		while (f_gets(line, sizeof(line), &file) && !strstr(line, "Delta Temperature"));

		// get start
		while (f_gets(line, sizeof(line), &file))
		{
			if (sscanf(line, "%f, %f, %f", &t, &ln, &temp) != 3)
				continue;


			if(ln >= lnStart)
			{
				startln = ln;
				starttemp = temp;
				break;
			}
		}

		// get end
		while (f_gets(line, sizeof(line), &file))
		{
			if (sscanf(line, "%f, %f, %f", &t, &ln, &temp) != 3)
				continue;


			if(ln >= lnEnd)
			{
				endln = ln;
				endtemp = temp;
				break;
			}
		}

		slope = (endtemp - starttemp) / (endln - startln);
    }


    printf("Slope = %f\n", slope);

    float power = atof(getMetaData(filename, META_POWER));
    float k = power / (4 * M_PI * slope *0.15) * calCoef;
    return k;
}

void get_boundries(float *minLn, float *maxLn, float *minTemp, float *maxTemp)
{
    FIL file;
    char line[64];
    float t, ln, temp;

    if (f_open(&file, filename, FA_READ) != FR_OK)
        return;

    // Skip header
    while (f_gets(line, sizeof(line), &file) && !strstr(line, "Delta Temperature"));

    // First data row
    if (f_gets(line, sizeof(line), &file) == 0) return;
    sscanf(line, "%f, %f, %f", &t, &ln, &temp);

    *minLn = *maxLn = ln;
    *minTemp = *maxTemp = temp;

    // Remaining rows
    while (f_gets(line, sizeof(line), &file))
    {
        if (sscanf(line, "%f, %f, %f", &t, &ln, &temp) == 3)
        {
            if (ln < *minLn) *minLn = ln;
            if (ln > *maxLn) *maxLn = ln;
            if (temp < *minTemp) *minTemp = temp;
            if (temp > *maxTemp) *maxTemp = temp;
        }
    }

    f_close(&file);
}

void drawAxis(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, float maxtemp, float minLnTime, float MaxLnTime)
{
	Displ_Line(x0, y0, x0, height, MAINTEXTCOLOUR);
	Displ_Line(x0, height, width, height, MAINTEXTCOLOUR);
}

void graph_draw_markers(GraphMarkers *m, uint16_t color1, uint16_t color2, uint8_t line)
{
    // Line 1
	if(line == 0 || line == 1) Displ_Line(m->x1, GRAPH_Y_MIN, m->x1, GRAPH_Y_MAX, color1);

    // Line 2
	if(line == 0 || line == 2) Displ_Line(m->x2, GRAPH_Y_MIN, m->x2, GRAPH_Y_MAX, color2);

    // Optional: highlight active line
    int16_t ax = (m->active == 0) ? m->x1 : m->x2;
    Displ_Line(ax, GRAPH_Y_MIN, ax, GRAPH_Y_MIN + 10, RED);
}

void graph_clear_markers(GraphMarkers *m, uint16_t x0, uint8_t line)
{
    // Overdraw with background color
	if(line == 0 || line == 1)
	{
	    Displ_Line(m->x1, GRAPH_Y_MIN, m->x1, GRAPH_Y_MAX, BACKGROUNDCOLOUR);

	    Displ_Pixel(m->x1, graph_yPoints[m->x1 - x0], SECONDARYTEXTCOLOUR);
	}
	if(line == 0|| line == 2)
	{
	    Displ_Line(m->x2, GRAPH_Y_MIN, m->x2, GRAPH_Y_MAX, BACKGROUNDCOLOUR);

	    Displ_Pixel(m->x2, graph_yPoints[m->x2 - x0], SECONDARYTEXTCOLOUR);
	}
}


float drawGraph(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height)
{
    get_boundries(&minLn, &maxLn, &minTemp, &maxTemp);

    float lnRange   = maxLn - minLn;
    float tempRange = maxTemp - minTemp;

    // Clear graph area
    Displ_FillArea(x0, y0, width, height, BACKGROUNDCOLOUR);

    // Draw axes
    Displ_Line(x0, y0 + height, x0 + width, y0 + height, MAINTEXTCOLOUR);
    Displ_Line(x0, y0, x0, y0 + height, MAINTEXTCOLOUR);

    // Open CSV again for streaming
    FIL file;
    char line[64];
    float t, ln, temp;

    if (f_open(&file, filename, FA_READ) != FR_OK)
        return 0;

    // Skip header
    while (f_gets(line, sizeof(line), &file) && !strstr(line, "Delta Temperature"));

    // Pixel window
    int px = 0;
//    float pxMinLn = minLn;
    float pxMaxLn = minLn + lnRange / width;

    // Accumulators for averaging
    float sumTemp = 0.0f;
    int countTemp = 0;

    while (f_gets(line, sizeof(line), &file))
    {
        if (sscanf(line, "%f, %f, %f", &t, &ln, &temp) != 3)
            continue;

        // Advance pixel window until ln fits
        while (ln > pxMaxLn && px < width)
        {
            if (countTemp > 0)
            {
                float avgTemp = sumTemp / (float)countTemp;

                int y = y0 + height -
                        (int)(((avgTemp - minTemp) / tempRange) * height);

                Displ_Pixel(x0 + px, y, SECONDARYTEXTCOLOUR);
                graph_yPoints[px] = y;
            }
            else graph_yPoints[px] = -1;

            // Move to next pixel column
            px++;
//            pxMinLn = minLn + (lnRange * px) / width;
            pxMaxLn = minLn + (lnRange * (px + 1)) / width;

            sumTemp = 0.0f;
            countTemp = 0;
        }

        // Accumulate for this pixel
        if (px < width)
        {
            sumTemp += temp;
            countTemp++;
        }
    }

    // Draw last pixel if needed
    if (countTemp > 0 && px < width)
    {
        float avgTemp = sumTemp / (float)countTemp;
        int y = y0 + height -
                (int)(((avgTemp - minTemp) / tempRange) * height);
        Displ_Pixel(x0 + px, y, SECONDARYTEXTCOLOUR);
    }

    f_close(&file);


    char buf[16];
	snprintf(buf, 16, "%.2f", minLn);
	Displ_WString(x0, y0+height+2, buf, Font16, 1, MAINTEXTCOLOUR, BACKGROUNDCOLOUR);
	snprintf(buf, 16, "%.2f", maxLn);
	Displ_WString(x0 + width - strlen(buf)*11, y0+height+2, buf, Font16, 1, MAINTEXTCOLOUR, BACKGROUNDCOLOUR);
	Displ_WString(x0 + width/2 - 6*11/2, y0+height+2, "LnTime", Font16, 1, SECONDARYTEXTCOLOUR, BACKGROUNDCOLOUR);

	// Print maxTemp at top of Y-axis
	snprintf(buf, sizeof(buf), "%.2f c", maxTemp);
	Displ_WString(x0 - (strlen(buf) * 11)/2, y0 - 17, buf, Font16, 1, MAINTEXTCOLOUR, BACKGROUNDCOLOUR);


    return lnRange / width;
}

void showGraphWithMarkers(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height)
{
	char buf[32];
    // 1. Draw the graph once
    float lnTPerPixel = drawGraph(x0, y0, width, height);
	float k=0;


    // 2. Initialize marker boundaries
    GRAPH_Y_MIN = y0;
    GRAPH_Y_MAX = y0 + height -1;

    float lnStart = atof(getMetaData(filename, META_REGION_START));
    float lnEnd   = atof(getMetaData(filename, META_REGION_END));

    float startPx = (lnStart - minLn) / lnTPerPixel;
    float endPx   = (lnEnd   - minLn) / lnTPerPixel;

    // 3. Initialize markers (you can adjust defaults later)
    GraphMarkers markers = {
        .x1 = x0 + (int16_t)startPx,
        .x2 = x0 + (int16_t)endPx,
        .active = 0
    };
	graph_draw_markers(&markers, YELLOW, ORANGE, 0);

    // 4. Main loop
    while (1)
    {
    	// Move left
    	if (buttons & (1 << 0)) { // B1
    	    HAL_Delay(debounceDelay);
    	    buttons = 0;

    	    do {
    	        graph_clear_markers(&markers, x0, markers.active ? 2 : 1);

    	        if (markers.active == 0 && markers.x1 > x0+1) markers.x1--;
    	        else if (markers.active == 1 && markers.x2 > markers.x1 + 1) markers.x2--;

    	        graph_draw_markers(&markers, YELLOW, ORANGE, markers.active ? 2 : 1);
    	        HAL_Delay(10);

    	    } while (!HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin));
    	}


    	// Move right
    	if (buttons & (1 << 3)) { // B4
    	    HAL_Delay(debounceDelay);
    	    buttons = 0;

    	    do {
    	        graph_clear_markers(&markers, x0, markers.active ? 2 : 1);

    	        if (markers.active == 0 && markers.x1 < markers.x2 - 1) markers.x1++;
    	        else if (markers.active == 1 && markers.x2 < (x0 + width - 1)) markers.x2++;

    	        graph_draw_markers(&markers, YELLOW, ORANGE, markers.active ? 2 : 1);
    	        HAL_Delay(10);

    	    } while (!HAL_GPIO_ReadPin(B4_GPIO_Port, B4_Pin));
    	}


		// Switch active line
		if (buttons & (1 << 2)) { // B3
			HAL_Delay(debounceDelay);
			buttons = 0;
			markers.active ^= 1;       // toggle 0 ↔ 1
			graph_draw_markers(&markers, YELLOW, ORANGE, 0);
		}

		// Calculate thermal conductivity
		if (buttons & (1 << 4)) { // B5
			HAL_Delay(debounceDelay);
			buttons = 0;
//			perform_slope_calculation(markers.x1, markers.x2);

			float start= (markers.x1 - x0)*lnTPerPixel + minLn, end=(markers.x2 - x0)*lnTPerPixel + minLn;
			snprintf(buf, 10, "%.4f", start);
			updateMetaData(filename, META_REGION_START, buf);
			snprintf(buf, 10, "%.4f", end);
			updateMetaData(filename, META_REGION_END, buf);

			snprintf(buf, 16, "k: %.4f W/mK", k);
			Displ_WString(480/2 - 11*strlen(buf)/2, 24+5, buf, Font16, 1, BACKGROUNDCOLOUR, BACKGROUNDCOLOUR);

			k = calculateK(start, end, 1, 1);

			snprintf(buf, 16, "k: %.4f W/mK", k);
			Displ_WString(480/2 - 11*strlen(buf)/2, 24+5, buf, Font16, 1, MAINTEXTCOLOUR, BACKGROUNDCOLOUR);

			snprintf(buf, 32, "  Start: %.3f Ln(s)  ", start);
			Displ_WString(480/2 - 11*strlen(buf)/2, 24+5+16, buf, Font16, 1, MAINTEXTCOLOUR, BACKGROUNDCOLOUR);
			snprintf(buf, 32, "  End: %.3f Ln(s)  ", end);
			Displ_WString(480/2 - 11*strlen(buf)/2, 24+5+16*2, buf, Font16, 1, MAINTEXTCOLOUR, BACKGROUNDCOLOUR);

			char kbuf[10];
			snprintf(kbuf, 10, "%.6f", k);
			updateMetaData(filename, META_CONDUCTIVITY, kbuf);
			snprintf(kbuf, 10, "%.6f", start);
			updateMetaData(filename, META_REGION_START, kbuf);
			snprintf(kbuf, 10, "%.6f", end);
			updateMetaData(filename, META_REGION_END, kbuf);
		}

		// Back
		if (buttons & (1 << 5)) { // B6
			HAL_Delay(debounceDelay);
			buttons = 0;
			break;
		}

        HAL_Delay(10); // small refresh delay
    }

    // Optional: clear markers when leaving
    Displ_FillArea(x0, y0, width, height, BACKGROUNDCOLOUR);
}

