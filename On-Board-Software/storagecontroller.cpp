#include "storagecontroller.h"
#include <cstdio>

#define DEBUG_READ_TEST_MSG

StorageController storageController("Storage Controller", 100);

//SD-Command-List
        // Abbreviation         // Index        // Functionality                        Argument        									Response
//-----------------------------------------------------------------------------------------------------------------------------------------------------
#define GO_IDLE_STATE           0b01000000      // Software reset.                      None(0)         									R1
#define SEND_OP_COND            0b01000001      // Initiate initialization process.     None(0)         									R1
#define SEND_IF_COND			0b01001000		// For SDCV2 only. Check voltage range.	Supply Voltage(1)[11:8], Check Pattern(0xAA)[7:0]	R7
#define APP_SEND_OP_COND        0b01101001      // Initiate initialization process(SD). Bit 30: HCS     									R1
#define SEND_CSD				0b01001001		// Read CSD register.					None(0)												R1
#define SEND_CID				0b01001010		// Read CID register.					None(0)												R1
#define STOP_TRANSMISSION		0b01001100		// Stop to read data.					None(0)												R1b
#define SET_BLOCKLEN            0b01010000      // Change R/W block size.               Block length   										R1
#define READ_SINGLE_BLOCK       0b01010001      // Read a block.                        Address[31:0]										R1
#define READ_MULTIPLE_BLOCK     0b01010010      // Read multiple blocks.                Address[31:0]										R1
#define WRITE_BLOCK             0b01011000      // Write a block.                       Address[31:0]   									R1
#define WRITE_MULTIPLE_BLOCK    0b01011001      // Write multiple blocks.               Address[31:0]   									R1
#define APP_CMD					0b01110111		// Leading command of ACMD<n> command.	None(0)												R1
#define READ_OCR				0b01111010		// Read OCR.							None(0)												R3
#define USE_CRC					0b01111011		// Use CRC.								Kp													R1 vermutlich

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

static uint8_t crc7(const uint8_t* data, uint8_t n)
{
	uint8_t crc = 0;
	for (uint8_t i = 0; i < n; i++)
	{
		uint8_t d = data[i];
		for (uint8_t j = 0; j < 8; j++)
		{
			crc <<= 1;
			if ((d & 0x80) ^ (crc & 0x80))
				crc ^= 0x09;

			d <<= 1;
		}
	}
	return (crc << 1) | 1;
}

static const uint16_t crctab[] = {
  0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
  0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
  0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
  0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
  0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
  0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
  0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
  0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
  0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
  0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
  0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
  0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
  0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
  0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
  0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
  0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
  0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
  0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
  0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
  0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
  0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
  0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
  0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
  0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
  0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
  0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
  0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
  0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
  0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
  0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
  0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
  0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
};

static uint16_t crc16(const uint8_t* data, size_t n)
{
	uint16_t crc = 0;
	for (size_t i = 0; i < n; i++)
		crc = crctab[(crc >> 8 ^ data[i]) & 0XFF] ^ (crc << 8);

	return crc;
}

StorageController::StorageController(const char *name, const long priority)  : Thread(name, priority)
{
	is_initialized = false;
}

int StorageController::sendCommand(uint8_t command, uint32_t argument, uint8_t crc, uint32_t *trailingData)
{
    uint8_t buff[6];
    buff[0] = command;
    *(uint32_t*)&buff[1] = swap32(argument);
    buff[5] = crc7(buff, 5);

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

	/*result = 0xFF;
	for (int i = 0; i < 10 && result != 0x01; i++)
	{
		spiHelper.selectSlave(SD);
		sendCommand(USE_CRC, 1);
		spiHelper.disableSlaves();
	}*/

	result = 0xFF;
	for (int i = 0; i < 10 && result != 0x01; i++)
	{
		uint32_t trailingData;

		spiHelper.selectSlave(SD);
		result = sendCommand(SEND_IF_COND, 0x1AA, 0x87, &trailingData);
		spiHelper.disableSlaves();

		PRINTF("CMD8-Response: %d, %x\n",  result, trailingData);
		suspendCallerUntil(NOW() + 1*MILLISECONDS);
	}

	result = 0xFF;
	uint32_t ocr;
	for (int i = 0; i < 100 && result != 0x01; i++)
	{
		spiHelper.selectSlave(SD);
		result = sendCommand(READ_OCR, 0, 0xFF, &ocr);
		spiHelper.disableSlaves();

		PRINTF("CMD58-Response: %d, %x\n",  result, ocr);
		suspendCallerUntil(NOW() + 1*MILLISECONDS);
	}

	result = 0xFF;
	for (int i = 0; i < 1000 && result != 0x00; i++)
	{
		uint32_t trailingData;
		spiHelper.selectSlave(SD);
		result = sendCommand(APP_CMD, 0, 0x65);
		//spi_bus.suspendUntilWriteFinished();
		//spiHelper.disableSlaves();

		//PRINTF("Result 2.5: %x\n", result);
		//suspendCallerUntil(NOW() + 1*MILLISECONDS);

		//spiHelper.selectSlave(SD);
		result = sendCommand(APP_SEND_OP_COND, 0x5180FFC0, 0x77, &trailingData);
		spi_bus.suspendUntilWriteFinished();
		spiHelper.disableSlaves();

		spi_bus.write(xff, 8);

		PRINTF("Result 3: %x, %x\n", result, trailingData);
		suspendCallerUntil(NOW() + 10*MILLISECONDS);
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
        if (sendCommand(READ_SINGLE_BLOCK, sector) == 0x00)
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

int StorageController::read_multiple_sector_segments(uint32_t sectorStart, uint32_t sectorCount, uint8_t *buf)
{
	int result = -1;

	spiHelper.selectSlave(SD);

	for (int i = 0; i < 10; i++)
	{
		if (sendCommand(READ_MULTIPLE_BLOCK, swap32(sectorStart)) == 0x00)
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

	for (int i = 0; i < sectorCount; i++)
	{
		result = -1;

		for (int j = 0; j < 1000; j++) {	//wait for data token
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
			PRINTF("No acknoledgement Nr. %d.\n", i);
			spiHelper.disableSlaves();
			return -1;
		}

		spi_bus.writeRead(xff, 512, buf + i*512, 512);

		spi_bus.write(xff, 2);                   //false 16-bits crc
	}

	result = -1;
	for (int i = 0; i < 10; i++)
	{
		if (sendCommand(STOP_TRANSMISSION, swap32(sectorStart)) == 0x00)
		{
			result = 0;
			break;
		}
	}

	if (result == -1)
	{
		PRINTF("Stop no ack.\n");
		spiHelper.disableSlaves();
		return -1;
	}

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

uint8_t buf[2048];

void StorageController::run()
{
    // TODO: may be necessary to wait for SD-card to initialize
	suspendCallerUntil(NOW() + 1*SECONDS);

	PRINTF("fail %d\n", initSD());

	#ifdef DEBUG_READ_TEST_MSG

	memset(buf, 0xFF, 2048);

	//PRINTF("read result: %d\n", write_sector_segment(0, buf));
	PRINTF("read result: %d\n", read_sector_segment(0, buf));
	for(int i = 0; i < 512; i++)
	{
		PRINTF("%x|%c\n", buf[i], buf[i]);
	}

	/*for (int i = 0; i < 1000000; i++)
	{
		int result = read_sector_segment(i, buf);
		if (result != 0 || buf[0] != 4)
			PRINTF("Sector: %d, Result: %d, Buf1: %d\n", i, result, buf[0]);
	}*/

	/*FATFS FatFs;
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
	 */

	/*while(1)
	{
		char buf[255] = "0:";
		scan_files(buf);
		suspendCallerUntil(NOW() + 1*SECONDS);
	}*/

	#endif

}
