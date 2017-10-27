#include "storagecontroller.h"
#include "sdspicard/sdspicard.h"
#include "fatfs/diskio.h"
#include <cstdio>
#include <cstdlib>
#include "murmur.h"
#include "telemetry.h"

//#define DEBUG_READ_TEST_MSG

StorageController storageController("Storage Controller", DEFAULT_THREAD_PRIORITY*2);

#ifndef DEBUG_READ_TEST_MSG
//Fifo<IMUdata, 100> imuFifo;
//Fifo<HkData, 100> hkFifo;
//Fifo<CmdData, 100> controlFifo;

//Subscriber imuFifoSub(IMUTopic, imuFifo);
//Subscriber hkFifoSub(hkTopic, hkFifo);
//Subscriber controlFifoSub(controlTopic, controlFifo);
#endif

char buf1[64];
#ifdef DEBUG_READ_TEST_MSG
char buf2[64];
#endif

const uint32_t wbSize = 2048;
uint8_t workBuf[wbSize] = {0};

//extern SdSpiCard sd;

void StorageController::format()
{
	formatSD = true;
	storageController.resume();
}

void StorageController::reset()
{
	resetSD = true;
	storageController.resume();
}

void StorageController::downlinkData()
{
	downlinkSD = true;
}

bool StorageController::isReady()
{
	return ready;
}

bool StorageController::isInitInProgress()
{
	return initInProgress;
}

bool StorageController::readFromFile(const TCHAR *fname, void *buffer, UINT len, BYTE mode)
{
	if (!ready) return false;

	FRESULT result;
	FIL fil;
	UINT readBytes;

	result = f_open(&fil, fname, FA_READ | mode);

	if (result != FR_OK) return false;

	result = f_read(&fil, buffer, len, &readBytes);
	f_close(&fil);

	if (result != FR_OK) return false;

	return true;

}

bool StorageController::writeToFile(const TCHAR *fname, void *buffer, UINT len, BYTE mode)
{
	if (len > 256)
		return false;

	writeFname = fname;
	writeMode = mode;

	memcpy(writeBuffer, buffer, len);
	writeLen = len;

	return true;
	/*
	if (!ready) return false;

	FRESULT result;
	FIL fil;
	UINT readBytes;

	result = f_open(&fil, fname, FA_WRITE | mode);

	if (result != FR_OK) return false;

	result = f_write(&fil, buffer, len, &readBytes);
	f_close(&fil);

	if (result != FR_OK) return false;

	return true;
	*/
}

int StorageController::getPckLen(DpGeneric *pack)
{
	switch(pack->sync)
	{
	case SYNC_DL_NF:
		return sizeof(DpFinfo);
	case SYNC_DL_FD:
		return sizeof(DpFdata);
	case SYNC_DL_FC:
		return sizeof(DpFend);
	case SYNC_DL_SC:
		return sizeof(DpDLcomplete);
	default:
		return -1;
	}
}

int StorageController::getPckCounter(DpGeneric *pack)
{
	switch(pack->sync)
	{
	case SYNC_DL_NF:
		return ((DpFinfo*)pack)->counter;
	case SYNC_DL_FD:
		return ((DpFdata*)pack)->counter;
	case SYNC_DL_FC:
		return ((DpFend*)pack)->counter;
	case SYNC_DL_SC:
		return ((DpDLcomplete*)pack)->counter;
	default:
		return -1;
	}
}

/*
bool StorageController::downlinkSDPck(DpGeneric *pack)
{
	uint32_t lastSavedPack = getPckCounter(&bufferedPacks[readPos]);
	if (lastSavedPack < currentPackage)
	{
		readPos = (readPos + currentPackage - lastSavedPack) % PACKBUF_SIZE;
		len -= currentPackage - lastSavedPack;
	}

	while (len >= PACKBUF_SIZE)
	{
		// repeat sending all Packs
		for (int i = readPos; i < readPos + len; i++)
		{
			DpGeneric *pack = &bufferedPacks[(readPos + i) % PACKBUF_SIZE];
			teleUART.write((const char*)pack, getPckLen(pack));
			suspendCallerUntil(NOW() + PACKSEND_DELAY*MILLISECONDS);
		}
		if (lastSavedPack < currentPackage)
		{
			readPos = (readPos + currentPackage - lastSavedPack) % PACKBUF_SIZE;
			len -= currentPackage - lastSavedPack;
		}
	}

	bufferedPacks[(readPos + len) % PACKBUF_SIZE] = *pack;
	len++;
	teleUART.write((const char*)pack, getPckLen(pack));
	suspendCallerUntil(NOW() + PACKSEND_DELAY*MILLISECONDS);

	return true;
}
*/

#define SD_SEND_TO_TIME 5000

/*
const unsigned char debug_dl_buf[55] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
	21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
	51, 52, 53, 54, 55};
	*/

void StorageController::sendSDdata()
{
	uint32_t currentPackage = 0;
	sdDLPackageTopic.publish(currentPackage);

	FRESULT result;
	DIR dir;
	FILINFO fno;
	FIL fil;

	result = f_opendir(&dir, "0:");
	PRINTF("SD-Downlink initialized %d\n", result);

	if (result == FR_OK)
	{
		while(1)
		{
			result = f_readdir(&dir, &fno);
			if (result != FR_OK || fno.fname[0] == 0)
				break;

			if (fno.fattrib & AM_DIR)
				continue; // ignore directories

			// downlink file name

			DpFinfo finfo;
			finfo.sync = SYNC_DL_NF;
			strcpy(finfo.fname, fno.fname);
			finfo.counter = currentPackage;
			//finfo.check = Murmur::mm_hash_32((uint8_t*)(&finfo), sizeof(finfo) - sizeof(finfo.check));

			//PRINTF("Downlink-Filename %s\n", finfo.fname);

			uint64_t stime = NOW();
			while((currentPackage != finfo.counter + 1) && (NOW() - stime < SD_SEND_TO_TIME*MILLISECONDS))
			{
				finfo.check = generateChecksum((uint8_t*)(&finfo), sizeof(finfo) - sizeof(finfo.check));
				teleUART.write((const char*)(&finfo), sizeof(finfo));
				suspendCallerUntil(NOW() + PACKSEND_DELAY*MILLISECONDS);
				uint32_t tmp_pck_cnt;
				sdDLPackageBuffer.get(tmp_pck_cnt);
				if (tmp_pck_cnt == currentPackage + 1)
					currentPackage = tmp_pck_cnt;
			}

			if (currentPackage != finfo.counter + 1) // timeout
			{
				PRINTF("Timeout\n");
				break;
			}

			result = f_open(&fil, fno.fname, FA_READ);
			//PRINTF("Downlink File %d\n", result);

			if (result == FR_OK)
			{
				DpFdata fdata;
				DpFend &fend = *(DpFend*)(&fdata);


				FSIZE_t fsize = fno.fsize;
				unsigned int bytes;

				//PRINTF("Size: %d\n", fsize);

				while (fsize >= 0)
				{
					f_read(&fil, fdata.fdata, /*sizeof(fdata.fdata)*/55, &bytes);
					fsize -= bytes;
					//PRINTF("RB: %d, Size: %d\n", bytes, fsize);

					if (bytes == 55/*sizeof(fdata.fdata)*/)
					{
						fdata.sync = SYNC_DL_FD;
						fdata.counter = currentPackage;

						//memcpy(fdata.fdata, debug_dl_buf, 55); // DEBUG
						//fdata.check = Murmur::mm_hash_32((uint8_t*)(&fdata), sizeof(DpFdata) - sizeof(fdata.check));

						//PRINTF("Downlink FD\n");
						uint64_t stime = NOW();
						while((currentPackage != fdata.counter + 1) && (NOW() - stime < SD_SEND_TO_TIME*MILLISECONDS))
						{
							fdata.check = generateChecksum((uint8_t*)(&fdata), sizeof(DpFdata) - sizeof(fdata.check));
							teleUART.write((const char*)(&fdata), sizeof(DpFdata));
							suspendCallerUntil(NOW() + PACKSEND_DELAY*MILLISECONDS);
							uint32_t tmp_pck_cnt;
							sdDLPackageBuffer.get(tmp_pck_cnt);
							if (tmp_pck_cnt == currentPackage + 1)
								currentPackage = tmp_pck_cnt;
						}

						if (currentPackage != fdata.counter + 1) // timeout
						{
							PRINTF("Timeout\n");
							f_close(&fil);
							//goto endDL;
							return;
						}
					}
					else
					{
						fend.sync = SYNC_DL_FC;
						fend.remaining_bytes = bytes;
						fend.counter = currentPackage;

						//memcpy(fend.fdata, debug_dl_buf, 54); // DEBUG
						//fend.check = Murmur::mm_hash_32((uint8_t*)(&fend), sizeof(DpFend) - sizeof(fend.check));

						//PRINTF("Size %d\n", sizeof(fend));
						//PRINTF("Downlink FC\n");
						uint64_t stime = NOW();
						while((currentPackage != fend.counter + 1) && (NOW() - stime < SD_SEND_TO_TIME*MILLISECONDS))
						{
							fend.check = generateChecksum((uint8_t*)(&fend), sizeof(DpFend) - sizeof(fend.check));
							teleUART.write((const char*)(&fend), sizeof(DpFend));
							suspendCallerUntil(NOW() + PACKSEND_DELAY*MILLISECONDS);
							uint32_t tmp_pck_cnt;
							sdDLPackageBuffer.get(tmp_pck_cnt);
							if (tmp_pck_cnt == currentPackage + 1)
								currentPackage = tmp_pck_cnt;
						}

						if (currentPackage != fend.counter + 1) // timeout
						{
							PRINTF("Timeout\n");
							f_close(&fil);
							//goto endDL;
							return;
						}
						break;
					}


				}
			}

			f_close(&fil);

		}

		PRINTF("Downlink Complete\n");

		DpDLcomplete dlComp;
		dlComp.sync = SYNC_DL_SC;
		dlComp.counter = currentPackage;
		uint64_t stime = NOW();
		while((currentPackage != dlComp.counter + 1) && (NOW() - stime < SD_SEND_TO_TIME*MILLISECONDS))
		{
			dlComp.check = generateChecksum((uint8_t*)(&dlComp), sizeof(dlComp) - sizeof(dlComp.check));
			teleUART.write((const char*)(&dlComp), sizeof(DpDLcomplete));
			suspendCallerUntil(NOW() + PACKSEND_DELAY*MILLISECONDS);
			uint32_t tmp_pck_cnt;
			sdDLPackageBuffer.get(tmp_pck_cnt);
			if (tmp_pck_cnt == currentPackage + 1)
				currentPackage = tmp_pck_cnt;
		}
	}

	//endDL:
	//f_closedir(&dir);
}

void StorageController::readFiles()
{
	FRESULT result;
	DIR dir;
	FILINFO fno;
	FIL fil;

	result = f_opendir(&dir, "0:");

	telemetry.dl_debug("Files:");

	if (result == FR_OK)
	{
		while(1)
		{
			result = f_readdir(&dir, &fno);
			if (result != FR_OK || fno.fname[0] == 0)
				break;

			if (fno.fattrib & AM_DIR)
				continue; // ignore directories

			telemetry.dl_debug(fno.fname);

			f_close(&fil);

			suspendCallerUntil(NOW() + 100*MILLISECONDS);

		}

		telemetry.dl_debug("File read complete\n");
	}
}

void StorageController::errorState()
{
	while(1)
	{
		initInProgress = false;
		suspendCallerUntil();

		if (formatSD == true)
		{
			initInProgress = true;
			//PRINTF("Format SD.\n");
			//PRINTF("Result mkfs: %d\n", f_mkfs("0:", FS_FAT32, 0, workBuf, wbSize));
			formatSD = false;
			resetSD = false;
			return;
		}
		else if (resetSD == true)
		{
			initInProgress = true;
			telemetry.dl_debug("Resumed.\n");
			resetSD = false;
			return;
		}
	}
}

void StorageController::run()
{
	//PRINTF("ST state 1\n");
	//storageController.suspendCallerUntil(NOW() + 0.5 * SECONDS);
	FATFS fs;
	FIL fil;       // File object
	FRESULT result;
	unsigned int bytes;

	#ifdef DEBUG_READ_TEST_MSG

	memset(buf1, 0, 63);

	//PRINTF("Mount: %d\n", f_mount(&fs, "0:", 0));

	PRINTF("Result mkfs: %d\n", f_mkfs("0:", FS_FAT32, 0, workBuf, wbSize));

	//PRINTF("SDbegin%d\n", sd.begin());

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

	char debugStr[64];
	storageControllerStatusTopic.publishConst(NO_INIT);

	while (1)
	{
		//PRINTF("ST state 2\n");
		result = f_mount(&fs, "0:", 1); // mount SD card
		//PRINTF("ST state 3\n");
		for (int i = 0; i < 5 && result != FR_OK; i++)
		{

			sprintf(debugStr, "SD not mounted %d.\n", result);
			PRINTF(debugStr);
			suspendCallerUntil(NOW() + 0.2*SECONDS);
			result = f_mount(&fs, "0:", 1);
		}

		sprintf(debugStr, "ST state 4: %d\n", result);
		//PRINTF(debugStr);

		// if mount not succesful, enter error state
		if (result != FR_OK)
		{
			storageControllerStatusTopic.publishConst(INIT_FAILED);
			//PRINTF("SD not mounted, no more retrys.\n");

			//OLD GOTO CODE
			/*error:
			initInProgress = false;
			suspendCallerUntil();

			if (formatSD == true)
			{
				initInProgress = true;
				telemetry.dl_debug("Format SD.\n");
				sprintf(debugStr, "Result mkfs: %d\n", f_mkfs("0:", FS_FAT32, 0, workBuf, wbSize));
				telemetry.dl_debug(debugStr);
				formatSD = false;
				resetSD = false;
				continue;
			}
			else if (resetSD == true)
			{
				initInProgress = true;
				telemetry.dl_debug("Resumed.\n");
				resetSD = false;
				continue;
			}
			else
				goto error;*/
			//NEW CODE
			errorState();
			continue;
			//NEW CODE END
		}

		//DEBUG
		/*while(1)
		{
			readFiles();
			suspendCallerUntil(NOW() + 1*SECONDS);
		}*/
		//DEBUG END

		// publish successful initialization
		initInProgress = false;
		storageControllerStatusTopic.publishConst(OK);

		//PRINTF("SD mounted.\n");

		int runcount = 1;

		// try to read runcount file
		result = f_open(&fil, "runcnt", FA_READ | FA_WRITE);

		while (result != FR_OK && result != FR_NO_FILE)
		{
			result = f_open(&fil, "runcnt", FA_READ | FA_WRITE);
			suspendCallerUntil(NOW() + 1*SECONDS);
		}

		//PRINTF("File runcount opened: %d.\n", result);

		if (result == FR_NO_FILE) // if no file found, create new one
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
		else if (result == FR_OK) // else, read runcount and increment
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

		// create file to write log data for current run
		sprintf(buf1, "log%d", runcount);
		result = f_open(&fil, buf1, FA_WRITE | FA_CREATE_ALWAYS);
		for (int i = 0; i < 10 && result != FR_OK; i++)
		{
			suspendCallerUntil(NOW() + 1*SECONDS);
			result = f_open(&fil, buf1, FA_WRITE | FA_CREATE_ALWAYS);
		}
		if (result != FR_OK)
		{
			storageControllerStatusTopic.publishConst(CRITICAL_ERROR);
			//goto error;
			errorState();
			continue;
		}

		//PRINTF("Open datfile result: %d", result);

		f_close(&fil);

		setPeriodicBeat(100*MILLISECONDS, 500*MILLISECONDS);

		ready = true;

		while(1)
		{
			//PRINTF("Run loop.\n");

			//PRINTF("In loop.\n");

			// check for incoming commands

			if (formatSD == true)
			{
				initInProgress = true;
				ready = false;
				telemetry.dl_debug("Format SD.\n");
				storageControllerStatusTopic.publishConst(NO_INIT);
				sprintf(debugStr, "Result mkfs: %d\n", f_mkfs("0:", FS_FAT32, 0, workBuf, wbSize));
				telemetry.dl_debug(debugStr);
				formatSD = false;
				resetSD = false;
				break;
			}
			else if (resetSD == true)
			{
				initInProgress = true;
				ready = false;
				storageControllerStatusTopic.publishConst(NO_INIT);
				resetSD = false;
				break;
			}

			if (downlinkSD == true)
			{
				storageControllerStatusTopic.publishConst(SD_DL_PROGRESS);
				sendSDdata();
				storageControllerStatusTopic.publishConst(OK);
				downlinkSD = false;
			}

			// write data from writeToFile function to SD card
			if (writeLen > 0)
			{
				FIL writeFil;
				UINT readBytes;

				result = f_open(&writeFil, writeFname, FA_WRITE | writeMode);

				if (result == FR_OK)
				{
					result = f_write(&writeFil, writeBuffer, writeLen, &readBytes);
					f_close(&writeFil);
				}

				writeLen = 0;
			}

			// open log file
			for (int i = 0; i < 10; i++)
			{
				result = f_open(&fil, buf1, FA_WRITE | FA_OPEN_APPEND);
				if (result == FR_OK)
					break;
				/*else
				{
					storageControllerStatusTopic.publishConst(CRITICAL_ERROR);
					//goto error;//PRINTF("File open try %d failed.\n", i);
				}*/
			}
			if (result != FR_OK)
			{
				storageControllerStatusTopic.publishConst(CRITICAL_ERROR);
				//suspendUntilNextBeat();
				//continue;
				errorState();
				break;
			}
			//PRINTF("Fopen success\n");

			// get all data from buffers and write to SD
			char imubyte = SYNC_IMU;
			char ptbyte = SYNC_PT;
			char calcbyte = SYNC_CALC;

			DownlinkIMU imu;
			DownlinkPresTemp hk;
			DownlinkCalc cmd;
			// Read Queues, write data to file
			//PRINTF("Write stuff\n");
			for (int i = 0; i < 10 && dlIMUFifo.get(imu); i++)
			{
				//PRINTF("IMU.\n");
				result = f_write(&fil, &imubyte, 1, &bytes);
				result = f_write(&fil, &imu, sizeof(imu), &bytes);
				//PRINTF("Write imu %d: %d\n", result, bytes);
			}
			for (int i = 0; i < 10 && dlPTFifo.get(hk); i++)
			{
				//PRINTF("PT.\n");
				result = f_write(&fil, &ptbyte, 1, &bytes);
				result = f_write(&fil, &hk, sizeof(hk), &bytes);
				//PRINTF("Write hk %d: %d\n", result, bytes);
			}
			for (int i = 0; i < 10 && dlCalcFifo.get(cmd); i++)
			{
				//PRINTF("CMD.\n");
				result = f_write(&fil, &calcbyte, 1, &bytes);
				result = f_write(&fil, &cmd, sizeof(cmd), &bytes);
				//PRINTF("Write cmd %d: %d\n", result, bytes);
			}
			//PRINTF("F closing %s.\n", buf1);
			f_close(&fil);
			//PRINTF("F closed.\n");

			suspendUntilNextBeat();
		}
	}
	#endif

}
