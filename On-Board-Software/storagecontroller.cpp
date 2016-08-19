#include "storagecontroller.h"
#include <cstdio>


#define DEBUG_READ_TEST_MSG

//StorageController storageController;

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

unsigned char buf[1024];

void StorageController::run()
{
    // TODO: may be necessary to wait for SD-card to initialize
	suspendCallerUntil(NOW() + 1*SECONDS);

	PRINTF("fail %d\n", sdcomm.init());

	#ifdef DEBUG_READ_TEST_MSG

	sdcomm.read_sector_segment(0, buf);
	for(int i = 0; i < 512; i++)
	{
		PRINTF("%x", buf[i] & 0xFF);
	}

	FATFS FatFs;
	FIL fil;       /* File object */

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
