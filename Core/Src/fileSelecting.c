/*
 * fileSelecting.c
 *
 *  Created on: Mar 18, 2026
 *      Author: cbyer
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileSelecting.h"
#include "main.h"
#include "z_displ_ILI9XXX.h"
#include "sd_functions.h"

uint16_t totalFiles = 0;

uint16_t count_csv_files(void)
{
    DIR dir;
    FILINFO fno;
    static char lfn[_MAX_LFN + 1];
    fno.lfname = lfn;
    fno.lfsize = sizeof(lfn);


    totalFiles = 0;

    if (f_opendir(&dir, "/") != FR_OK)
        return 0;

    while (1)
    {
        if (f_readdir(&dir, &fno) != FR_OK || fno.fname[0] == 0)
            break;

        if (fno.fattrib & AM_DIR)
            continue;

        char *name = fno.fname;
        int len = strlen(name);

        if (len > 4 && strcasecmp(&name[len - 4], ".csv") == 0)
            totalFiles++;
    }

    f_closedir(&dir);
    return totalFiles;
}

#define PAGE_SIZE 10
#define MAX_NAME_LEN 64

char pageFiles[PAGE_SIZE][MAX_NAME_LEN];
uint16_t pageStart = 0;   // index of first file on this page
uint8_t pageCount = 0;    // how many files are actually in this page

void load_page(uint16_t startIndex)
{
    DIR dir;
    FILINFO fno;
    static char lfn[_MAX_LFN + 1];
    fno.lfname = lfn;
    fno.lfsize = sizeof(lfn);


    pageStart = startIndex;
    pageCount = 0;

    if (f_opendir(&dir, "/") != FR_OK)
        return;

    uint16_t csvIndex = 0;

    while (1)
    {
        if (f_readdir(&dir, &fno) != FR_OK || fno.fname[0] == 0)
            break;

        if (fno.fattrib & AM_DIR)
            continue;

        char *name = (fno.lfname && fno.lfname[0]) ? fno.lfname : fno.fname;
//        char *name = fno.fname;
        int len = strlen(name);

        if (len > 4 && strcasecmp(&name[len - 4], ".csv") == 0)
        {
            if (csvIndex >= pageStart && csvIndex < pageStart + PAGE_SIZE)
            {
                strncpy(pageFiles[pageCount], name, MAX_NAME_LEN);
                pageCount++;
            }

            csvIndex++;

            if (pageCount >= PAGE_SIZE)
                break;
        }
    }

    f_closedir(&dir);
}

uint8_t selected = 0;  // 0..pageCount-1

void draw_page(void)
{
	Displ_CLS(BLACK);

    for (int i = 0; i < pageCount; i++)
    {
        uint16_t color = (i == selected) ? YELLOW : WHITE;
        Displ_WString(10, 20 + i * 22, pageFiles[i], Font16, 1, color, BLACK);
    }

    char buf[32];
    int page = pageStart / PAGE_SIZE + 1;
    int totalPages = (totalFiles + PAGE_SIZE - 1) / PAGE_SIZE;

    snprintf(buf, sizeof(buf), "Page %d/%d", page, totalPages);
    Displ_WString(10, 260, buf, Font16, 1, CYAN, BLACK);
}


// Button 0 = up
// Button 1 = select
// Button 2 = down
// Button 3 = next page
// Button 4 = prev. page
// Button 5 = back
int debounceDelay = 150;
void selectFile(void)
{
	totalFiles = count_csv_files();
	pageStart = 0;
	load_page(pageStart);

	selected = 0;

	draw_page();

	while(1)
	{
		// UP
		if(buttons & 0x01)
		{
			HAL_Delay(debounceDelay);
			buttons = 0;
			if(selected > 0)
			{
				selected--;
			}
			else if(pageStart > 0)
			{
				pageStart -= PAGE_SIZE;
				load_page(pageStart);
				selected = PAGE_SIZE - 1;
				if(selected >= pageCount) selected = pageCount - 1;
			}

			draw_page();
		}

		// DOWN
		if(buttons & 0x04)
		{
			HAL_Delay(debounceDelay);
			buttons = 0;
			if(selected < pageCount - 1)
			{
				selected++;
			}
			else if(pageStart + PAGE_SIZE < totalFiles)
			{
				pageStart += PAGE_SIZE;
				load_page(pageStart);
				selected = 0;
			}

			draw_page();
		}

		// SELECT
		if(buttons & 0x02)
		{
			buttons = 0;
			HAL_Delay(debounceDelay);
			sprintf(filename, "%s", pageFiles[selected]);
			break;
		}

		// BACK
		if(buttons & 0x10)
		{
			buttons = 0;
			HAL_Delay(debounceDelay);
			break;

		}
	}
}
