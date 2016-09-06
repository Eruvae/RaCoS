/* Arduino SdSpiCard Library
 * Copyright (C) 2012 by William Greiman
 *
 * This file is part of the Arduino SdSpiCard Library
 *
 * This Library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the Arduino SdSpiCard Library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "rodos.h"
#include "SdSpiCard.h"
#include "../comminterfaces.h"
#include "../spihelper.h"

// debug trace macro
#define SD_TRACE(m, b)
//#define SD_TRACE(m, b) PRINTF("%s: %d\n", m, b);
#define SD_CS_DBG(m)
// #define SD_CS_DBG(m) Serial.println(F(m));
#define USE_SD_CRC 2

//==============================================================================

#if USE_SD_CRC
// CRC functions
//------------------------------------------------------------------------------
static uint8_t CRC7(const uint8_t* data, uint8_t n)
{
	uint8_t crc = 0;
	for (uint8_t i = 0; i < n; i++)
	{
		uint8_t d = data[i];
		for (uint8_t j = 0; j < 8; j++)
		{
			crc <<= 1;
			if ((d & 0x80) ^ (crc & 0x80))
			{
				crc ^= 0x09;
			}
			d <<= 1;
		}
	}
	return (crc << 1) | 1;
}
//------------------------------------------------------------------------------
#if USE_SD_CRC == 1
// slower CRC-CCITT
// uses the x^16,x^12,x^5,x^1 polynomial.
static uint16_t CRC_CCITT(const uint8_t *data, size_t n)
{
	uint16_t crc = 0;
	for (size_t i = 0; i < n; i++)
	{
		crc = (uint8_t)(crc >> 8) | (crc << 8);
		crc ^= data[i];
		crc ^= (uint8_t)(crc & 0xff) >> 4;
		crc ^= crc << 12;
		crc ^= (crc & 0xff) << 5;
	}
	return crc;
}
#elif USE_SD_CRC > 1  // CRC_CCITT
//------------------------------------------------------------------------------
// faster CRC-CCITT
// uses the x^16,x^12,x^5,x^1 polynomial.
#ifdef __AVR__
static const uint16_t crctab[] PROGMEM = {
#else  // __AVR__
static const uint16_t crctab[] = {
#endif  // __AVR__
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
static uint16_t CRC_CCITT(const uint8_t* data, size_t n)
{
	uint16_t crc = 0;
	for (size_t i = 0; i < n; i++)
	{
		#ifdef __AVR__
		crc = pgm_read_word(&crctab[(crc >> 8 ^ data[i]) & 0XFF]) ^ (crc << 8);
		#else  // __AVR__
		crc = crctab[(crc >> 8 ^ data[i]) & 0XFF] ^ (crc << 8);
		#endif  // __AVR__
	}
	return crc;
}
#endif  // CRC_CCITT
#endif  // USE_SD_CRC

//==============================================================================
// SdSpiCard member functions
//------------------------------------------------------------------------------
bool SdSpiCard::begin()
{
	m_errorCode = m_type = 0;
	//m_spi = spi;
	//m_chipSelectPin = chipSelectPin;
	// 16-bit init start time allows over a minute
	uint64_t t0 = NOW();
	uint32_t arg;

	// initialize SPI bus and chip select pin.
	//spiBegin(m_chipSelectPin);

	// set SCK rate for initialization commands.
	spi_bus.config(SPI_PARAMETER_BAUDRATE, SPI_INIT_BAUDRATE);
	//m_sckDivisor = SPI_SCK_INIT_DIVISOR;

	// toggle chip select and set slow SPI clock.
	//chipSelectLow();
	//chipSelectHigh();
	spiHelper.selectSlave(SD);
	spiHelper.disableSlaves();

	// must supply min of 74 clock cycles with CS high.
	spi_bus.write(xff, 10);
	//for (uint8_t i = 0; i < 10; i++) {
	//  spiSend(0XFF);
	//}
	// command to go idle in SPI mode
	while (cardCommand(CMD0, 0) != R1_IDLE_STATE)
	{
		if ((NOW() - t0) > SD_INIT_TIMEOUT) {
			error(SD_CARD_ERROR_CMD0);
			spiHelper.disableSlaves();
			return false;
		}
	}
	//PRINTF("CMD0 successful\n");
	#if USE_SD_CRC
	if (cardCommand(CMD59, 1) != R1_IDLE_STATE)
	{
		error(SD_CARD_ERROR_CMD59);
		spiHelper.disableSlaves();
		return false;
	}
	#endif  // USE_SD_CRC
	// check SD version
	while (1)
	{
		if (cardCommand(CMD8, 0x1AA) == (R1_ILLEGAL_COMMAND | R1_IDLE_STATE))
		{
			type(SD_CARD_TYPE_SD1);
			break;
		}
		for (uint8_t i = 0; i < 4; i++)
		{
			spi_bus.writeRead(xff, 1, &m_status, 1);
		}
		//PRINTF("Status: %x\n", m_status);
		if (m_status == 0XAA)
		{
			type(SD_CARD_TYPE_SD2);
			break;
		}
		if ((NOW() - t0) > SD_INIT_TIMEOUT)
		{
			error(SD_CARD_ERROR_CMD8);
			spiHelper.disableSlaves();
			return false;
		}
	}
	// initialize card and send host supports SDHC if SD2
	arg = type() == SD_CARD_TYPE_SD2 ? 0X40000000 : 0;

	//spiHelper.disableSlaves(); // added for test
	while (cardAcmd(ACMD41, arg) != R1_READY_STATE)
	{
		// check for timeout
		if ((NOW() - t0) > SD_INIT_TIMEOUT)
		{
			PRINTF("ACMD41 Timeout\n");
			error(SD_CARD_ERROR_ACMD41);
			spiHelper.disableSlaves();
			return false;
		}
		//spiHelper.disableSlaves(); // added for test
		//Thread::suspendCallerUntil(NOW() + 1*MILLISECONDS); // added for test
	}
	// if SD2 read OCR register to check for SDHC card
	if (type() == SD_CARD_TYPE_SD2)
	{
		if (cardCommand(CMD58, 0))
		{
			error(SD_CARD_ERROR_CMD58);
			spiHelper.disableSlaves();
			return false;
		}
		uint8_t receive;
		spi_bus.writeRead(xff, 1, &receive, 1);
		if ((receive & 0XC0) == 0XC0)
		{
			type(SD_CARD_TYPE_SDHC);
		}
		// Discard rest of ocr - contains allowed voltage range.
		spi_bus.write(xff, 3);
	}

	spiHelper.disableSlaves();
	spi_bus.config(SPI_PARAMETER_BAUDRATE, SPI_BAUDRATE);
	return true;
}

//------------------------------------------------------------------------------
// send command and return error code.  Return zero for OK
uint8_t SdSpiCard::cardCommand(uint8_t cmd, uint32_t arg)
{
	// select card
	if (!spiHelper.isSlaveSelected(SD))
	{
		spiHelper.selectSlave(SD);
	}
	// wait if busy
	waitNotBusy(SD_WRITE_TIMEOUT);

	uint8_t buf[6];
	// form message
	buf[0] = (uint8_t)0x40U | cmd;
	buf[1] = (uint8_t)(arg >> 24U);
	buf[2] = (uint8_t)(arg >> 16U);
	buf[3] = (uint8_t)(arg >> 8U);
	buf[4] = (uint8_t)arg;
	#if USE_SD_CRC
	// add CRC
	buf[5] = CRC7(buf, 5);
	#else  // USE_SD_CRC
	buf[5] = cmd == CMD0 ? 0x95 : 0x87;
	#endif  // USE_SD_CRC

	// send message
	spi_bus.write(buf, 6);

	// skip stuff byte for stop read
	if (cmd == CMD12)
		spi_bus.write(xff, 1);

	// wait for response
	for (uint8_t i = 0; i != 0XFF; i++)
	{
	  spi_bus.writeRead(xff, 1, &m_status, 1);
	  if (!(m_status & 0x80))
		  break;
	}
	return m_status;
}

//------------------------------------------------------------------------------
uint32_t SdSpiCard::cardSize()
{
	csd_t csd;
	if (!readCSD(&csd))
	{
		return 0;
	}
	if (csd.v1.csd_ver == 0)
	{
		uint8_t read_bl_len = csd.v1.read_bl_len;
		uint16_t c_size = (csd.v1.c_size_high << 10) | (csd.v1.c_size_mid << 2) | csd.v1.c_size_low;
		uint8_t c_size_mult = (csd.v1.c_size_mult_high << 1) | csd.v1.c_size_mult_low;
		return (uint32_t)(c_size + 1) << (c_size_mult + read_bl_len - 7);
	}
	else if (csd.v2.csd_ver == 1)
	{
		uint32_t c_size = 0X10000L * csd.v2.c_size_high + 0X100L * (uint32_t)csd.v2.c_size_mid + csd.v2.c_size_low;
		return (c_size + 1) << 10;
	}
	else
	{
		error(SD_CARD_ERROR_BAD_CSD);
		return 0;
	}
}

//------------------------------------------------------------------------------
bool SdSpiCard::erase(uint32_t firstBlock, uint32_t lastBlock)
{
	csd_t csd;
	if (!readCSD(&csd))
	{
		spiHelper.disableSlaves();
		return false;
	}
	// check for single block erase
	if (!csd.v1.erase_blk_en)
	{
		// erase size mask
		uint8_t m = (csd.v1.sector_size_high << 1) | csd.v1.sector_size_low;
		if ((firstBlock & m) != 0 || ((lastBlock + 1) & m) != 0)
		{
			// error card can't erase specified area
			error(SD_CARD_ERROR_ERASE_SINGLE_BLOCK);
			spiHelper.disableSlaves();
			return false;
		}
	}
	if (m_type != SD_CARD_TYPE_SDHC)
	{
		firstBlock <<= 9;
		lastBlock <<= 9;
	}
	if (cardCommand(CMD32, firstBlock) || cardCommand(CMD33, lastBlock) || cardCommand(CMD38, 0))
	{
		error(SD_CARD_ERROR_ERASE);
		spiHelper.disableSlaves();
		return false;
	}
	if (!waitNotBusy(SD_ERASE_TIMEOUT))
	{
		error(SD_CARD_ERROR_ERASE_TIMEOUT);
		spiHelper.disableSlaves();
		return false;
	}

	spiHelper.disableSlaves();
	return true;
}
//------------------------------------------------------------------------------
bool SdSpiCard::eraseSingleBlockEnable()
{
	csd_t csd;
	return readCSD(&csd) ? csd.v1.erase_blk_en : false;
}
//------------------------------------------------------------------------------
bool SdSpiCard::isBusy()
{
	bool rtn = true;
	bool selected = spiHelper.isSlaveSelected(SD);
	if (!selected)
	{
		spiHelper.selectSlave(SD);
	}
	for (uint8_t i = 0; i < 8; i++)
	{
		uint8_t rec;
		spi_bus.writeRead(xff, 1, &rec, 1);
		if (0xFF == rec)
		{
			rtn = false;
			break;
		}
	}
	if (!selected)
	{
		spiHelper.disableSlaves();
	}
	return rtn;
}
//------------------------------------------------------------------------------
bool SdSpiCard::readBlock(uint32_t blockNumber, uint8_t* dst) {
	SD_TRACE("RB", blockNumber);
	// use address if not SDHC card
	if (type() != SD_CARD_TYPE_SDHC)
	{
		PRINTF("Ne, die ist HC!\n");
		blockNumber <<= 9;
	}
	if (cardCommand(CMD17, blockNumber))
	{
		error(SD_CARD_ERROR_CMD17);
		spiHelper.disableSlaves();
		return false;
	}
	if (!readData(dst, 512))
	{
		spiHelper.disableSlaves();
		return false;
	}
	spiHelper.disableSlaves();
	return true;
}
//------------------------------------------------------------------------------
bool SdSpiCard::readBlocks(uint32_t block, uint8_t* dst, size_t count) {
	if (!readStart(block))
	{
		return false;
	}
	for (uint16_t b = 0; b < count; b++, dst += 512)
	{
		if (!readData(dst, 512))
		{
			return false;
		}
	}
	return readStop();
}
//------------------------------------------------------------------------------
bool SdSpiCard::readData(uint8_t *dst) {
	if (!readData(dst, 512))
	{
		return false;
	}
	return true;
}
//------------------------------------------------------------------------------
bool SdSpiCard::readData(uint8_t* dst, size_t count)
{
	#if USE_SD_CRC
	uint16_t crc;
	#endif  // USE_SD_CRC
	// wait for start block token
	uint64_t t0 = NOW();
	spi_bus.writeRead(xff, 1, &m_status, 1);
	while (m_status == 0XFF)
	{
		if ((NOW() - t0) > SD_READ_TIMEOUT)
		{
			error(SD_CARD_ERROR_READ_TIMEOUT);
			spiHelper.disableSlaves();
			return false;
		}
		spi_bus.writeRead(xff, 1, &m_status, 1);
	}
	if (m_status != DATA_START_BLOCK)
	{
		error(SD_CARD_ERROR_READ);
		spiHelper.disableSlaves();
		return false;
	}
	// transfer data

	int read_cnt = spi_bus.writeRead(xff, count, dst, count);
	if (read_cnt != count)
	{
		m_status = 0xFF;
		error(SD_CARD_ERROR_SPI_DMA);
		spiHelper.disableSlaves();
		return false;
	}

	#if USE_SD_CRC
	// get crc
	spi_bus.writeRead(xff, 2, (uint8_t*)&crc, 2);
	crc = swap16(crc);
	if (crc != CRC_CCITT(dst, count))
	{
		error(SD_CARD_ERROR_READ_CRC);
		spiHelper.disableSlaves();
		return false;
	}
	#else
	// discard crc
	spi_bus.write(xff, 2);
	#endif  // USE_SD_CRC
	return true;
}

//------------------------------------------------------------------------------
bool SdSpiCard::readOCR(uint32_t* ocr)
{
	if (cardCommand(CMD58, 0))
	{
		error(SD_CARD_ERROR_CMD58);
		spiHelper.disableSlaves();
		return false;
	}
	spi_bus.writeRead(xff, 4, (uint8_t*)ocr, 4);
	*ocr = swap32(*ocr);

	spiHelper.disableSlaves();
	return true;
}
//------------------------------------------------------------------------------
/** read CID or CSR register */
bool SdSpiCard::readRegister(uint8_t cmd, void* buf)
{
	uint8_t* dst = reinterpret_cast<uint8_t*>(buf);
	if (cardCommand(cmd, 0))
	{
		error(SD_CARD_ERROR_READ_REG);
		spiHelper.disableSlaves();
		return false;
	}
	if (!readData(dst, 16))
	{
		spiHelper.disableSlaves();
		return false;
	}
	spiHelper.disableSlaves();
	return true;
}

//------------------------------------------------------------------------------
bool SdSpiCard::readStart(uint32_t blockNumber)
{
	SD_TRACE("RS", blockNumber);
	if (type() != SD_CARD_TYPE_SDHC)
	{
		blockNumber <<= 9;
	}
	if (cardCommand(CMD18, blockNumber))
	{
		error(SD_CARD_ERROR_CMD18);
		spiHelper.disableSlaves();
		return false;
	}
	return true;
}

//------------------------------------------------------------------------------
bool SdSpiCard::readStop()
{
	if (cardCommand(CMD12, 0))
	{
		error(SD_CARD_ERROR_CMD12);
		spiHelper.disableSlaves();
		return false;
	}
	spiHelper.disableSlaves();
	return true;
}
//------------------------------------------------------------------------------
// wait for card to go not busy
bool SdSpiCard::waitNotBusy(uint64_t timeoutNanos)
{
	uint64_t t0 = NOW();
	uint8_t response;
	spi_bus.writeRead(xff, 1, &response, 1);
	while (response != 0XFF)
	{
		if ((NOW() - t0) >= timeoutNanos)
		{
			return false;
		}
		spi_bus.writeRead(xff, 1, &response, 1);
	}
	return true;
}
//------------------------------------------------------------------------------
bool SdSpiCard::writeBlock(uint32_t blockNumber, const uint8_t* src)
{
	SD_TRACE("WB", blockNumber);
	// use address if not SDHC card
	if (type() != SD_CARD_TYPE_SDHC)
	{
		blockNumber <<= 9;
	}
	if (cardCommand(CMD24, blockNumber))
	{
		error(SD_CARD_ERROR_CMD24);
		spiHelper.disableSlaves();
		return false;
	}
	if (!writeData(DATA_START_BLOCK, src))
	{
		spiHelper.disableSlaves();
		return false;
	}

	#define CHECK_PROGRAMMING 0
	#if CHECK_PROGRAMMING
	// wait for flash programming to complete
	if (!waitNotBusy(SD_WRITE_TIMEOUT))
	{
		error(SD_CARD_ERROR_WRITE_TIMEOUT);
		spiHelper.disableSlaves();
		return false;
	}
	// response is r2 so get and check two bytes for nonzero
	uint8_t rec;
	spi_bus.writeRead(xff, 1, &rec, 1);
	if (cardCommand(CMD13, 0) || rec)
	{
		error(SD_CARD_ERROR_WRITE_PROGRAMMING);
		spiHelper.disableSlaves();
		return false;
	}
	#endif  // CHECK_PROGRAMMING

	spiHelper.disableSlaves();
	return true;
}
//------------------------------------------------------------------------------
bool SdSpiCard::writeBlocks(uint32_t block, const uint8_t* src, size_t count)
{
	if (!writeStart(block, count))
	{
		spiHelper.disableSlaves();
		return false;
	}
	for (size_t b = 0; b < count; b++, src += 512)
	{
		if (!writeData(src))
		{
			spiHelper.disableSlaves();
			return false;
		}
	}
	return writeStop();
}

//------------------------------------------------------------------------------
bool SdSpiCard::writeData(const uint8_t* src)
{
	// wait for previous write to finish
	if (!waitNotBusy(SD_WRITE_TIMEOUT))
	{
		error(SD_CARD_ERROR_WRITE_TIMEOUT);
		spiHelper.disableSlaves();
		return false;
	}
	if (!writeData(WRITE_MULTIPLE_TOKEN, src))
	{
		spiHelper.disableSlaves();
		return false;
	}
	return true;
}

//------------------------------------------------------------------------------
// send one block of data for write block or write multiple blocks
bool SdSpiCard::writeData(uint8_t token, const uint8_t* src)
{
	#if USE_SD_CRC
	uint16_t crc = swap16(CRC_CCITT(src, 512));
	#else  // USE_SD_CRC
	uint16_t crc = 0XFFFF;
	#endif  // USE_SD_CRC

	spi_bus.write(&token, 1);
	spi_bus.write(src, 512);
	spi_bus.write((uint8_t*)&crc, 2);

	spi_bus.writeRead(xff, 1, &m_status, 1);
	if ((m_status & DATA_RES_MASK) != DATA_RES_ACCEPTED)
	{
		error(SD_CARD_ERROR_WRITE);
		spiHelper.disableSlaves();
		return false;
	}
	return true;
}
//------------------------------------------------------------------------------
bool SdSpiCard::writeStart(uint32_t blockNumber, uint32_t eraseCount) {
	SD_TRACE("WS", blockNumber);
	// send pre-erase count
	if (cardAcmd(ACMD23, eraseCount))
	{
		error(SD_CARD_ERROR_ACMD23);
		spiHelper.disableSlaves();
		return false;
	}

	// use address if not SDHC card
	if (type() != SD_CARD_TYPE_SDHC)
	{
		blockNumber <<= 9;
	}

	if (cardCommand(CMD25, blockNumber))
	{
		error(SD_CARD_ERROR_CMD25);
		spiHelper.disableSlaves();
		return false;
	}
	return true;
}

//------------------------------------------------------------------------------
bool SdSpiCard::writeStop()
{
	if (!waitNotBusy(SD_WRITE_TIMEOUT))
	{
		error(SD_CARD_ERROR_STOP_TRAN);
		spiHelper.disableSlaves();
		return false;
	}
	uint8_t token = STOP_TRAN_TOKEN;
	spi_bus.write(&token, 1);
	if (!waitNotBusy(SD_WRITE_TIMEOUT))
	{
		error(SD_CARD_ERROR_STOP_TRAN);
		spiHelper.disableSlaves();
		return false;
	}
	spiHelper.disableSlaves();
	return true;
}
