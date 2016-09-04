#include "storagecontroller.h"
#include "sdspicard/sdspicard.h"
#include "fatfs/diskio.h"
#include <cstdio>
#include <cstdlib>

#define DEBUG_READ_TEST_MSG

//StorageController storageController("Storage Controller", 100);

FRESULT scan_files (char* path /* Start node to be scanned (***also used as work area***) */)
{
    FRESULT res;
    DIR dir;
    UINT i;
    static FILINFO fno;



    res = f_opendir(&dir, path);                       /* Open the directory */
    PRINTF("Res: %d\n", res);
    if (res == FR_OK) {
        while (1)
        {
            res = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (fno.fattrib & AM_DIR) {                    /* It is a directory */
                i = strlen(path);
                sprintf(&path[i], "/%s", fno.fname);
                res = scan_files(path);                    /* Enter the directory */
                if (res != FR_OK) break;
                path[i] = 0;
            } else {                                       /* It is a file. */
                PRINTF("%s/%s\n", path, fno.fname);
            }
        }
        f_closedir(&dir);
    }

    return res;
}

char readBuf[64];
char readBuf2[64];

const uint32_t wbSize = 2048;
uint8_t workBuf[wbSize] = {0};

void StorageController::run()
{
    // TODO: may be necessary to wait for SD-card to initialize
	suspendCallerUntil(NOW() + 1*SECONDS);

	FATFS fs;
	FIL fil;       // File object
	FRESULT result;
	unsigned int readBytes;

	#ifdef DEBUG_READ_TEST_MSG

	memset(readBuf, 0, 64);

	//PRINTF("Mount: %d\n", f_mount(&fs, "0:", 0));

	//PRINTF("Result mkfs: %d\n", f_mkfs("0:", FS_FAT32, 0, workBuf, wbSize));

	PRINTF("Mount: %d\n", f_mount(&fs, "0:", 0));

	PRINTF("Open: %d", f_open(&fil, "message.txt", FA_READ));
	f_read(&fil, readBuf, 64, &readBytes);
	readBuf[readBytes] = '\0';

	f_open(&fil, "datafile.dat", FA_WRITE | FA_CREATE_ALWAYS);
	f_write(&fil, "Deine Mutter!", 14, &readBytes);
	f_close(&fil);
	f_open(&fil, "datafile.dat", FA_READ);
	f_read(&fil, readBuf2, 64, &readBytes);

	while(1)
	{
		//teleUART.write(readBuf, readBytes);
		PRINTF("%s; %d\n", readBuf, readBytes);
		PRINTF("%s; %d\n", readBuf2, readBytes);
		suspendCallerUntil(NOW() + 1*SECONDS);
	}

	#endif

	result = f_mount(&fs, "0:", 0);

	if (result != FR_OK)
	{
		// send error code to healthwatchdog
	}

	int runcount = 1;

	result = f_open(&fil, "runcount.txt", FA_READ | FA_WRITE);

	if (result == FR_NO_FILE)
	{
		f_close(&fil);
		result = f_open(&fil, "runcount.txt", FA_WRITE | FA_CREATE_ALWAYS);
		f_write(&fil, "1", 1, &readBytes);
	}
	else
	{
		f_read(&fil, readBuf, 64, &readBytes);
		readBuf[readBytes] = 0;
		runcount = atoi(readBuf);
		if (runcount == 0)
		{
			// Fehler
		}
		runcount++;
		sprintf(readBuf, "%d", runcount);
		f_close(&fil);
		result = f_open(&fil, "runcount.txt", FA_WRITE | FA_CREATE_ALWAYS);
		f_write(&fil, readBuf, sizeof(readBuf), &readBytes);
	}
	f_close(&fil);

	sprintf(readBuf, "data_run%d.dat", runcount);
	f_open(&fil, readBuf, FA_WRITE | FA_CREATE_ALWAYS);

	setPeriodicBeat(100*MILLISECONDS, 500*MILLISECONDS);

	while(1)
	{
		//IMUdata imu;
		//HkData hk;
		//CmdData cmd;
		// Read Queues, write data to file
		/*while (imuFifo.get(imu))
		{

		}*/

		suspendUntilNextBeat();
	}

}
