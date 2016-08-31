#include "storagecontroller.h"
#include "sdspicard/sdspicard.h"
#include <cstdio>

#define DEBUG_READ_TEST_MSG

StorageController storageController("Storage Controller", 100);

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

uint8_t buf[2048];

void StorageController::run()
{
	memset(buf, 0xFF, 2048);
    // TODO: may be necessary to wait for SD-card to initialize
	suspendCallerUntil(NOW() + 1*SECONDS);


	/*
	SdSpiCard sd;

	bool suc = sd.begin();

	PRINTF("Init success: %d\n", suc);

	//suc = sd.cardSize();

	//PRINTF("SD-Size: %d\n", suc);

	suc = sd.readBlock(0, buf);

	PRINTF("Read success: %d; error: %d\n", suc, sd.errorCode());

	for(int i = 0; i < 512; i++)
	{
		PRINTF("%x|%c\n", buf[i], buf[i]);
	}
	*/

	//PRINTF("fail %d\n", initSD());

	#ifdef DEBUG_READ_TEST_MSG

	//PRINTF("read result: %d\n", write_sector_segment(0, buf));
	/*PRINTF("read result: %d\n", read_sector_segment(0, buf));
	for(int i = 0; i < 512; i++)
	{
		PRINTF("%x|%c\n", buf[i], buf[i]);
	}*/

	/*for (int i = 0; i < 1000000; i++)
	{
		int result = read_sector_segment(i, buf);
		if (result != 0 || buf[0] != 4)
			PRINTF("Sector: %d, Result: %d, Buf1: %d\n", i, result, buf[0]);
	}*/


	FATFS FatFs;
	FIL fil;       // File object

	PRINTF("Mount: %d\n", f_mount(&FatFs, "0:", 0));

	//f_mount(&FatFs, "0:", 0);

	unsigned int readBytes;

	PRINTF("Open: %d", f_open(&fil, "message.txt", FA_READ));
	f_read(&fil, readBuf, 64, &readBytes);
	readBuf[readBytes] = '\0';

	while(1)
	{
		//teleUART.write(readBuf, readBytes);
		PRINTF("%s; %d\n", readBuf, readBytes);
		suspendCallerUntil(NOW() + 1*SECONDS);
	}



	/*while(1)
	{
		char buf[255] = "0:";
		scan_files(buf);
		suspendCallerUntil(NOW() + 1*SECONDS);
	}*/

	#endif

}
