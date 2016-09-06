#include "storagecontroller.h"
#include "sdspicard/sdspicard.h"
#include "fatfs/diskio.h"
#include <cstdio>
#include <cstdlib>
#include "healthwatchdog.h"

//#define DEBUG_READ_TEST_MSG

StorageController storageController("Storage Controller", 100);

#ifndef DEBUG_READ_TEST_MSG
Fifo<IMUdata, 100> imuFifo;
Fifo<HkData, 100> hkFifo;
Fifo<CmdData, 100> controlFifo;

Subscriber imuFifoSub(IMUTopic, imuFifo);
Subscriber hkFifoSub(hkTopic, hkFifo);
Subscriber controlFifoSub(controlTopic, controlFifo);
#endif

char buf1[64];
#ifdef DEBUG_READ_TEST_MSG
char buf2[64];
#endif

//const uint32_t wbSize = 2048;
//uint8_t workBuf[wbSize] = {0};

//extern SdSpiCard sd;

void StorageController::run()
{
	FATFS fs;
	FIL fil;       // File object
	FRESULT result;
	unsigned int bytes;

	#ifdef DEBUG_READ_TEST_MSG

	memset(buf1, 0, 63);

	//PRINTF("Mount: %d\n", f_mount(&fs, "0:", 0));

	//PRINTF("Result mkfs: %d\n", f_mkfs("0:", FS_FAT32, 0, workBuf, wbSize));

	PRINTF("SDbegin%d\n", sd.begin());

	PRINTF("Mount: %d\n", f_mount(&fs, "0:", 0));

	PRINTF("Open: %d", f_open(&fil, "message.txt", FA_READ));
	f_read(&fil, buf1, 63, &bytes);
	buf1[bytes] = '\0';

	f_open(&fil, "datafile.dat", FA_WRITE | FA_CREATE_ALWAYS);
	f_write(&fil, "Test write.", 14, &bytes);
	f_close(&fil);
	f_open(&fil, "datafile.dat", FA_READ);
	f_read(&fil, buf2, 63, &bytes);

	while(1)
	{
		//teleUART.write(buf1, bytes);
		PRINTF("%s; %d\n", buf1, bytes);
		PRINTF("%s; %d\n", buf2, bytes);
		suspendCallerUntil(NOW() + 1*SECONDS);
	}

	#else

	result = f_mount(&fs, "0:", 1);

	for (int i = 0; i < 5 && result != FR_OK; i++)
	{
		PRINTF("SD not mounted.\n");
		suspendCallerUntil(NOW() + 1*SECONDS);
		result = f_mount(&fs, "0:", 1);
	}

	if (result != FR_OK)
	{
		healthWatchdog.setStorageControllerStatus(INIT_FAILED);
		PRINTF("SD not mounted, no more retrys.\n");
		suspendCallerUntil();
	}

	healthWatchdog.setStorageControllerStatus(OK);

	//PRINTF("SD mounted.\n");

	int runcount = 1;

	result = f_open(&fil, "runcnt", FA_READ | FA_WRITE);

	while (result != FR_OK && result != FR_NO_FILE)
	{
		result = f_open(&fil, "runcnt", FA_READ | FA_WRITE);
		suspendCallerUntil(NOW() + 1*SECONDS);
	}

	//PRINTF("File runcount opened: %d.\n", result);

	if (result == FR_NO_FILE)
	{
		//PRINTF("Create new.\n");
		f_close(&fil);
		result = f_open(&fil, "runcnt", FA_WRITE | FA_CREATE_ALWAYS);
		while (result != FR_OK)
		{
			suspendCallerUntil(NOW() + 1*SECONDS);
			result = f_open(&fil, "runcnt", FA_WRITE | FA_CREATE_ALWAYS);
		}
		f_write(&fil, "1", 1, &bytes);
	}
	else if (result == FR_OK)
	{
		//PRINTF("Use existing.\n");
		bytes = 0;
		result = f_read(&fil, buf1, sizeof(buf1) - 1, &bytes);
		f_close(&fil);
		//PRINTF("Result runcount read: %d, bytes: %d\n", result, bytes);
		buf1[bytes] = 0;
		//PRINTF("Hallo: %s\n", buf1);
		runcount = atoi(buf1);
		//PRINTF("Hallo: %d\n", runcount);
		if (runcount == 0)
		{
			PRINTF("Runcount NAN.\n");
			// Fehler (evtl)
		}
		runcount++;
		bytes = sprintf(buf1, "%d", runcount);
		buf1[bytes] = 0;
		//PRINTF("bytes: %d\n", bytes);
		//PRINTF("Hallo: %s\n", buf1);
		result = f_open(&fil, "runcnt", FA_WRITE | FA_CREATE_ALWAYS);
		while (result != FR_OK)
		{
			suspendCallerUntil(NOW() + 1*SECONDS);
			result = f_open(&fil, "runcnt", FA_WRITE | FA_CREATE_ALWAYS);
		}
		//PRINTF("Result runcount write: %d\n", result);
		f_write(&fil, buf1, strlen(buf1), &bytes);
	}
	f_close(&fil);

	//PRINTF("Sprintf filename.\n");
	sprintf(buf1, "log%d", runcount);
	result = f_open(&fil, buf1, FA_WRITE | FA_CREATE_ALWAYS);
	while (result != FR_OK)
	{
		suspendCallerUntil(NOW() + 1*SECONDS);
		result = f_open(&fil, buf1, FA_WRITE | FA_CREATE_ALWAYS);
	}

	//PRINTF("Open datfile result: %d", result);

	f_close(&fil);

	setPeriodicBeat(100*MILLISECONDS, 500*MILLISECONDS);

	while(1)
	{

		//PRINTF("In loop.\n");

		for (int i = 0; i < 10; i++)
		{
			result = f_open(&fil, buf1, FA_WRITE);
			if (result == FR_OK)
				break;
			else
				PRINTF("File open try %d failed.\n", i);
		}
		if (result != FR_OK)
		{
			suspendUntilNextBeat();
			continue;
		}

		char imubyte = SYNC_IMU;
		char ptbyte = SYNC_PT;
		char calcbyte = SYNC_CALC;

		IMUdata imu;
		HkData hk;
		CmdData cmd;
		// Read Queues, write data to file
		while (imuFifo.get(imu))
		{
			f_write(&fil, &imubyte, 1, &bytes);
			f_write(&fil, &imu, sizeof(imu), &bytes);
		}
		while (hkFifo.get(hk))
		{
			f_write(&fil, &ptbyte, 1, &bytes);
			f_write(&fil, &hk, sizeof(hk), &bytes);
		}
		while (controlFifo.get(cmd))
		{
			f_write(&fil, &calcbyte, 1, &bytes);
			f_write(&fil, &cmd, sizeof(cmd), &bytes);
		}

		f_close(&fil);


		suspendUntilNextBeat();
	}
	#endif

}
