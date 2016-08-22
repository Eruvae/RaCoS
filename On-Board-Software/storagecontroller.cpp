#include "storagecontroller.h"
#include <cstdio>

#define DEBUG_READ_TEST_MSG

StorageController storageController;

//SD-Command-List
        // Abbreviation         // Index        // Functionality                        Argument        									Response
//-----------------------------------------------------------------------------------------------------------------------------------------------------
#define GO_IDLE_STATE           0b01000000      // Software reset.                      None(0)         									R1
#define SEND_OP_COND            0b01000001      // Initiate initialization process.     None(0)         									R1
#define SEND_IF_COND			0b01001000		// For SDCV2 only. Check voltage range.	Supply Voltage(1)[11:8], Check Pattern(0xAA)[7:0]	R7
#define APP_SEND_OP_COND        0b01101001      // Initiate initialization process(SD). Bit 30: HCS     									R1
#define SET_BLOCKLEN            0b01010000      // Change R/W block size.               Block length   										R1
#define READ_SINGLE_BLOCK       0b01010001      // Read a block.                        Address[31:0]										R1
#define READ_MULTIPLE_BLOCK     0b01010010      // Read multiple blocks.                Address[31:0]										R1
#define WRITE_BLOCK             0b01011000      // Write a block.                       Address[31:0]   									R1
#define WRITE_MULTIPLE_BLOCK    0b01011001      // Write multiple blocks.               Address[31:0]   									R1
#define APP_CMD					0b01110111		// Leading command of ACMD<n> command.	None(0)												R1
#define READ_OCR				0b01111010		// Read OCR.							None(0)												R3

const uint8_t xff[512] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
						  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
						  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
						  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
						  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
						  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
						  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
						  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
						  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
						  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
						  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
						  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
						  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
						  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
						  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
						  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
						  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
						  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
						  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
						  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
						  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
						  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
						  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
						  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
						  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
						  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
						  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
						  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
						  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
						  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
						  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
						  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

StorageController::StorageController()
{
	is_initialized = false;
}

int StorageController::sendCommand(uint8_t command, uint32_t argument, uint8_t crc, uint32_t *trailingData)
{
    uint8_t buff[6];
    buff[0] = command;
    *(uint32_t*)&buff[1] = argument;
    buff[5] = crc;

    //spiHelper.selectSlave(SD);

    int ret;
    if ((ret = spi_bus.write(buff, 6)) < 6)
    {
    	//spiHelper.disableSlaves();
    	return -1;
    }

    int result = -1;
    uint8_t ff = 0xFF;
    for (int i = 0; i < 32; i++)	//wait for response
    {
        uint8_t rec;
        spi_bus.writeRead(xff, 1, &rec, 1);
        if (rec != 0xFF) // Respond received
        {
            result = rec & 0xFF;
            if (trailingData != NULL)
            {
            	spi_bus.writeRead(xff, 4, (uint8_t*)trailingData, 4);
            }
            break;
        }
    }

    //spiHelper.disableSlaves();

    return result;
}

int StorageController::initSD()
{

    // TODO: Might be necessary to change Baudrate/Clock Speed; maybe sleep
	int result = 0xFF;
	for (int i = 0; i < 10 && result != 0x01; i++)
	{
		spiHelper.selectSlave(SD);
		result = sendCommand(GO_IDLE_STATE, 0, 0x95);
		spiHelper.disableSlaves();

		PRINTF("Result 1: %x\n", result);
		suspendCallerUntil(NOW() + 1*MILLISECONDS);
	}
	if (result != 0x01)
	{
		return -1;
	}
	PRINTF("Result 2: %x\n", result);

	result = 0xFF;
	for (int i = 0; i < 10 && result != 0x01; i++)
	{
		uint32_t trailingData;

		spiHelper.selectSlave(SD);
		result = sendCommand(SEND_IF_COND, 0xAA010000, 0x87, &trailingData);
		spiHelper.disableSlaves();

		PRINTF("CMD8-Response: %d, %x\n",  result, trailingData);
		suspendCallerUntil(NOW() + 1*MILLISECONDS);
	}

	/*result = 0xFF;
	for (int i = 0; i < 100 && result != 0x01; i++)
	{
		uint32_t trailingData;
		result = sendCommand(READ_OCR, 0, 0xFF, &trailingData);
		PRINTF("CMD58-Response: %d, %x\n",  result, trailingData);
		suspendCallerUntil(NOW() + 1*MILLISECONDS);
	}*/

	result = 0xFF;
	for (int i = 0; i < 10000 && result != 0x00; i++)
	{
		spiHelper.selectSlave(SD);
		result = sendCommand(APP_CMD, 0, 0x65);
		spiHelper.disableSlaves();

		PRINTF("Result 2.5: %x\n", result);
		suspendCallerUntil(NOW() + 1*MILLISECONDS);

		spiHelper.selectSlave(SD);
		result = sendCommand(APP_SEND_OP_COND, 0x00000040, 0x77);
		spiHelper.disableSlaves();

		PRINTF("Result 3: %x\n", result);
		suspendCallerUntil(NOW() + 1*MILLISECONDS);
	}
	//PRINTF("Result: %x\n", result);

    if (result != 0x00)
    {
    	return -2;
    }

    result = 0xFF;
	for (int i = 0; i < 100 && result != 0x00; i++)
	{
		uint32_t trailingData;

		spiHelper.selectSlave(SD);
		result = sendCommand(READ_OCR, 0, 0xFF, &trailingData);
		spiHelper.disableSlaves();

		PRINTF("CMD58-Response: %d, %x\n",  result, trailingData);
		suspendCallerUntil(NOW() + 1*MILLISECONDS);
	}

    /*if ((result = sendCommand(SET_BLOCKLEN, swap32(512))) != 0x00)
    {
    	PRINTF("Result 4: %x\n", result);
    	return -3;
    }*/

    is_initialized = true;
    return 0;

}

int StorageController::get_status()
{
	if (!is_initialized)
		return -1;

	return 0;
}

int StorageController::read_sector_segment(uint32_t sector, uint8_t *buf)
{
    int result = -1;

    spiHelper.selectSlave(SD);

    for (int i = 0; i < 10; i++)
    {
        if (sendCommand(READ_SINGLE_BLOCK, swap32(sector)) == 0x00)
        {
            result = 0;
            break;
        }
    }

    if (result == -1)
    {
    	PRINTF("Failed send RSB-Command\n");
    	spiHelper.disableSlaves();
        return -1;
    }

    result = -1;

    for (int i = 0; i < 1000; i++) {	//wait for data token
        uint8_t rec;
        spi_bus.writeRead(xff, 1, &rec, 1);
        //PRINTF("%x", rec & 0xFF);
        if ((rec & 0xFF) == 0xFE) // Respond received
        {
            result = 0;
            break;
        }
    }

    if (result == -1)
    {
    	PRINTF("No acknoledgement.\n");
        spiHelper.disableSlaves();
        return -1;
    }

    spi_bus.writeRead(xff, 512, buf, 512);

    spi_bus.write(xff, 2);                   //false 16-bits crc

    spiHelper.disableSlaves();

    return 0;
}

int StorageController::write_sector_segment(uint32_t sector, const uint8_t *buf)
{
    int result = -1;

    spiHelper.selectSlave(SD);

    for (int i = 0; i < 10; i++)
    {
        if (sendCommand(WRITE_BLOCK, swap32(sector), 0xFF) == 0x00)
        {
            result = 0;
            break;
        }
    }

    if (result == -1)
    {
    	spiHelper.disableSlaves();
        return -1;
    }

    uint8_t token = 0xFE;

    spi_bus.write(xff, 1);
    spi_bus.write(&token, 1);

    spi_bus.write(buf, 512);

    spi_bus.write(xff, 2); // false 16-bit crc

    spiHelper.disableSlaves();
    return 0;
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

uint8_t buf[1024];

void StorageController::run()
{
    // TODO: may be necessary to wait for SD-card to initialize
	suspendCallerUntil(NOW() + 1*SECONDS);

	PRINTF("fail %d\n", initSD());

	#ifdef DEBUG_READ_TEST_MSG

	memset(buf, 0xFF, 1024);

	//PRINTF("read result: %d\n", write_sector_segment(0, buf));
	/*PRINTF("read result: %d\n", read_sector_segment(2, buf));
	for(int i = 0; i < 512; i++)
	{
		PRINTF("%x|%c\n", buf[i], buf[i]);
	}*/
	for (int i = 0; i < 1000000; i++)
	{
		int result = read_sector_segment(i, buf);
		if (result != 0 || buf[0] != 4)
			PRINTF("Sector: %d, Result: %d, Buf1: %d\n", i, result, buf[0]);
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
