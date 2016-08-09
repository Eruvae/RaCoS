#include "storagecontroller.h"

#define DEBUG_READ_TEST_MSG

StorageController::StorageController()
{
}

FRESULT scan_files (char* path /* Start node to be scanned (***also used as work area***) */)
{
    FRESULT res;
    DIR dir;
    UINT i;
    static FILINFO fno;


    res = f_opendir(&dir, path);                       /* Open the directory */
    if (res == FR_OK) {
        while (1)
        {
            res = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (fno.fattrib & AM_DIR) {                    /* It is a directory */
                i = strlen(path);
                //sprintf(&path[i], "/%s", fno.fname);
                res = scan_files(path);                    /* Enter the directory */
                if (res != FR_OK) break;
                path[i] = 0;
            } else {                                       /* It is a file. */
                //printf("%s/%s\n", path, fno.fname);
            }
        }
        f_closedir(&dir);
    }

    return res;
}

char readBuf[64];

void StorageController::run()
{
    // TODO: may be necessary to wait for SD-card to initialize
	suspendCallerUntil(NOW() + 1*SECONDS);
	sdcomm.init();

	#ifdef DEBUG_READ_TEST_MSG

	FATFS FatFs;
	FIL fil;       /* File object */


	f_mount(&FatFs, "0:", 0);

	unsigned int readBytes;

	f_open(&fil, "message.txt", FA_READ);
	f_read(&fil, readBuf, 64, &readBytes);

	while(1)
	{
		teleUART.write(readBuf, readBytes);
		suspendCallerUntil(NOW() + 1*SECONDS);
	}

	#endif

}
