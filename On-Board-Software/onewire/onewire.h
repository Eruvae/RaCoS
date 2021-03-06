/*
 * onewire.h
 *
 *  Created on: 30.08.2016
 *      Author: TobiZ
 */

#ifndef ONEWIRE_ONEWIRE_H_
#define ONEWIRE_ONEWIRE_H_

#include "rodos.h"

#define SEARCH_ROM          0xF0    // Identify ROM-Codes of slave device
#define READ_ROM            0x33    // Only use if only one slave device, read ROM-Code
#define MATCH_ROM           0x55    // followed by 64-Bit ROM-Code to adress device
#define SKIP_ROM            0xCC    // Adress all slaves simultanously (e.g. init conversion), don't use READ_SCRATCHPAD afterwards if multiple slaves
#define ALARM_SEARCH        0xEC    // SEARCH_ROM, but only slaves with set alarm flag answer

#define CONVERT_T           0x44    // Initiates temperature conversion, DS18B20 transmits conversion status to master (not applicable for parasitepowered DS18B20s)
#define WRITE_SCRATCHPAD    0x4E    // Writes data into scratchpad bytes 2, 3, and 4 (TH, TL, and configuration registers), Master transmits 3 data bytes to DS18B20
#define READ_SCRATCHPAD     0xBE    // Reads the entire scratchpad including the CRC byte, DS18B20 transmits up to 9 data bytes to master
#define COPY_SCRATCHPAD     0x48    // Copies TH, TL, and configuration register data from the scratchpad to EEPROM
#define RECALL_E            0xB8    // Recalls TH, TL, and configuration register data from EEPROM to the scratchpad, DS18B20 transmits recall status to master
#define READ_POWER_SUPPLY   0xB4    // Signals DS18B20 power supply mode to the master, DS18B20 transmits supply status to master

class OneWire
{
private:
	HAL_UART *onewire_uart;

	unsigned char ROM_NO[8];
	uint8_t LastDiscrepancy;
	uint8_t LastFamilyDiscrepancy;
	uint8_t LastDeviceFlag;

	/** Read a single bit over OneWire UART
	 *
	 * \return The read bit (1 or 0)
	 */
	uint8_t readBit();

	/** Write a single bit over OneWire UART
	 *
	 * \param[in] The bit to write (1 or 0)
	 */
	void writeBit(uint8_t bit);

public:
	OneWire(HAL_UART *onewire_uart) : onewire_uart(onewire_uart), LastDiscrepancy(0),  LastFamilyDiscrepancy(0), LastDeviceFlag(0){}

	/** Initiate the OneWire UART and config to OneWire-Mode
	 *
	 * has to be called before communication
	 */
	void init();

	/** Perform a OneWire reset cycle
	 *
	 * \return true if device present, false if not
	 */
	bool reset();

	/** Read a single byte from OneWire-Bus
	 *
	 * \return The read byte (0x00 to 0xFF)
	 */
	uint8_t readByte();

	/** Write a single byte via OneWire-Bus
	 *
	 * \param[in] The byte to write (0x00 to 0xFF)
	 */
	void writeByte(uint8_t byte);

	/** Read multiple bytes from OneWire-Bus
	 *
	 * \param[in] The buffer to put the recieved bytes in
	 * \param[in] The number of bytes to read
	 */
	void read(void *recBuf, int len);

	/** Read multiple bytes from OneWire-Bus
	 *
	 * \param[in] The buffer to write
	 * \param[in] The number of bytes to write
	 */
	void write(const void *sendBuf, int len);

	uint8_t crc8(const void *vptr, int len);

	void readROM(uint8_t *rom);

	void selectROM(const uint8_t rom[8]);

	void skipROM();

	void convertT();

	void readScratchpad(void *recBuf);

	void writeScratchpad(const uint8_t th, const uint8_t tl, const uint8_t config);

	// Clear the search state so that if will start from the beginning again.
	void reset_search();

	// Setup the search to find the device type 'family_code' on the next call
	// to search(*newAddr) if it is present.
	void target_search(uint8_t family_code);

	// Look for the next device. Returns 1 if a new address has been
	// returned. A zero might mean that the bus is shorted, there are
	// no devices, or you have already retrieved all of them.  It
	// might be a good idea to check the CRC to make sure you didn't
	// get garbage.  The order is deterministic. You will always get
	// the same devices in the same order.
	uint8_t search(uint8_t *newAddr, bool search_mode = true);


};

#endif /* ONEWIRE_ONEWIRE_H_ */
