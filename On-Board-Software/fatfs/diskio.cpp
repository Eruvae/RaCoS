/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
//#include "../healthwatchdog.h"
#include "../sdspicard/sdspicard.h"

/* Definitions of physical drive number for each drive */
#define DEV_SD		0

SdSpiCard sd;


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	if (pdrv == DEV_SD)
	{
		result = sd.errorCode();

		if (result == SD_CARD_ERROR_INIT_NOT_CALLED)
			stat = STA_NOINIT;
		else if (result != 0)
			stat = STA_PROTECT;
		else
			stat = 0;

		return stat;
	}
	return STA_NODISK;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	bool result;

	if (pdrv == DEV_SD)
	{
		result = sd.begin();

		stat = result == true ? 0 : STA_NOINIT;

		return stat;

	}
	return STA_NODISK;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res = RES_PARERR;
	bool result = false;

	if (pdrv == DEV_SD)
	{
		// translate the arguments here

		if (count == 1)
			result = sd.readBlock(sector, buff);
		else if (count > 1)
			result = sd.readBlocks(sector, buff, count);
		else
			return RES_PARERR;

		if (result == false)
			res = RES_ERROR;
		else
			res = RES_OK;

		return res;

	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res = RES_PARERR;
	bool result = false;

	if (pdrv == DEV_SD)
	{
		// translate the arguments here

		if (count == 1)
			result = sd.writeBlock(sector, buff);
		else if (count > 1)
		/*{
			for (int i = 0; i < count; i++)
				sd.writeBlock(sector + i, buff);
		}*/
			result = sd.writeBlocks(sector, buff, count);
		else
			return RES_PARERR;

		if (result == false)
			res = RES_ERROR;
		else
			res = RES_OK;

		return res;

	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	//PRINTF("disk_ioctl called\n");
	DRESULT res = RES_PARERR;
	int result;

	if (pdrv == DEV_SD)
	{
		if (cmd == CTRL_SYNC)
			res = RES_OK;

		else if (cmd == GET_SECTOR_COUNT)
		{
			*(uint32_t*)buff = sd.cardSize();
			if (*(uint32_t*)buff > 0)
			{
				res = RES_OK;

			}
			else
			{
				PRINTF("ioctl card size fault\n");
				res = RES_ERROR;
			}
		}

		else if (cmd == GET_SECTOR_SIZE)
		{
			*(uint32_t*)buff = 512;
			res = RES_OK;
		}

		else if (cmd == GET_BLOCK_SIZE)
		{
			*(uint32_t*)buff = 1;
			res = RES_OK;
		}

		else if (cmd == CTRL_TRIM)
		{
			result = sd.erase(((uint32_t*)buff)[0], ((uint32_t*)buff)[1]);
			if (result == true)
				res = RES_OK;
			else
				res = RES_ERROR;
		}

		return res;
	}

	return RES_PARERR;
}

