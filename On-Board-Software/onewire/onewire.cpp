/*
 * onewire.cpp
 *
 *  Created on: 30.08.2016
 *      Author: TobiZ
 */

#include "onewire.h"

static const uint8_t dscrc_table[] = {
    0,94,188,226,97,63,221,131,194,156,126,32,163,253,31,65,
    157,195,33,127,252,162,64,30,95,1,227,189,62,96,130,220,
    35,125,159,193,66,28,254,160,225,191,93,3,128,222,60,98,
    190,224,2,92,223,129,99,61,124,34,192,158,29,67,161,255,
    70,24,250,164,39,121,155,197,132,218,56,102,229,187,89,7,
    219,133,103,57,186,228,6,88,25,71,165,251,120,38,196,154,
    101,59,217,135,4,90,184,230,167,249,27,69,198,152,122,36,
    248,166,68,26,153,199,37,123,58,100,134,216,91,5,231,185,
    140,210,48,110,237,179,81,15,78,16,242,172,47,113,147,205,
    17,79,173,243,112,46,204,146,211,141,111,49,178,236,14,80,
    175,241,19,77,206,144,114,44,109,51,209,143,12,82,176,238,
    50,108,142,208,83,13,239,177,240,174,76,18,145,207,45,115,
    202,148,118,40,171,245,23,73,8,86,180,234,105,55,213,139,
    87,9,235,181,54,104,138,212,149,203,41,119,244,170,72,22,
    233,183,85,11,136,214,52,106,43,117,151,201,74,20,246,168,
    116,42,200,150,21,75,169,247,182,232,10,84,215,137,107,53
};

void OneWire::init()
{
	onewire_uart->init(9600); // possiblly 115200 (doesnt matter)
	onewire_uart->config(UART_PARAMETER_MODE, UART_MODE_ONEWIRE);
}

bool OneWire::reset()
{
	onewire_uart->config(UART_PARAMETER_BAUDRATE, 9600);
	while(onewire_uart->getcharNoWait() != -1);		// Empty Receive Buffer
	onewire_uart->putcharNoWait(0xF0);				// Send Reset Impulse
	onewire_uart->suspendUntilWriteFinished();
	uint8_t result = onewire_uart->getcharNoWait();	// Get Presence Pulse

	//PRINTF("Presence: %d\n", result);

	if (result != 0xF0)
		return true;

	return false;
}

uint8_t OneWire::readBit()
{
	onewire_uart->config(UART_PARAMETER_BAUDRATE, 115200);
	while(onewire_uart->getcharNoWait() != -1);		// Empty Receive Buffer
	onewire_uart->putcharNoWait(0xFF);
	onewire_uart->suspendUntilWriteFinished();
	uint8_t result = onewire_uart->getcharNoWait();	// Get Bit

	//PRINTF("Bit: %d\n", result);

	if (result == 0xFF)
		return 1;

	return 0;
}

void OneWire::writeBit(uint8_t bit)
{
	onewire_uart->config(UART_PARAMETER_BAUDRATE, 115200);
	while(onewire_uart->getcharNoWait() != -1);		// Empty Receive Buffer

	if (bit == 0)
		onewire_uart->putcharNoWait(0);
	else
		onewire_uart->putcharNoWait(0xFF);

	onewire_uart->suspendUntilWriteFinished();
	onewire_uart->getcharNoWait();	// Empty written Bit from Buffer
}

uint8_t OneWire::readByte()
{
    uint8_t bitMask;
    uint8_t byte = 0;

    for (bitMask = 0x01; bitMask; bitMask <<= 1)
    	if (readBit()) byte |= bitMask;

    return byte;
}

void OneWire::writeByte(uint8_t byte)
{
    uint8_t bitMask;

    for (bitMask = 0x01; bitMask; bitMask <<= 1)
    	writeBit((bitMask & byte) ? 1 : 0);
}

void OneWire::read(void *recBuf, int len)
{
	for (int i = 0; i < len; i++)
		((uint8_t*)recBuf)[i] = readByte();
}

void OneWire::write(const void *sendBuf, int len)
{
	for (int i = 0; i < len; i++)
	    writeByte(((const uint8_t*)sendBuf)[i]);
}

uint8_t OneWire::crc8(const void *vptr, int len)
{
    uint8_t *addr = (uint8_t*)vptr;
    /*uint8_t crc = 0;

    while (len--)
        crc = dscrc_table[crc ^ *addr++];

    return crc;*/

    uint8_t crc = 0;
	while (len--)
	{
		uint8_t inbyte = *addr++;
		for (uint8_t i = 8; i; i--) {
			uint8_t mix = (crc ^ inbyte) & 0x01;
			crc >>= 1;
			if (mix) crc ^= 0x8C;
			inbyte >>= 1;
		}
	}
	return crc;
}

void OneWire::readROM(uint8_t *rom)
{
	writeByte(READ_ROM);
	read(rom, 8);
}

void OneWire::selectROM(const uint8_t rom[8])
{
	writeByte(MATCH_ROM);	// Choose ROM
	write(rom, 8);
}

void OneWire::skipROM()
{
	writeByte(SKIP_ROM);	// Skip ROM
}

void OneWire::convertT()
{
	writeByte(CONVERT_T);
}

void OneWire::readScratchpad(void *recBuf)
{
	writeByte(READ_SCRATCHPAD);
	read(recBuf, 9);
}

void OneWire::writeScratchpad(const uint8_t th, const uint8_t tl, const uint8_t config)
{
	writeByte(WRITE_SCRATCHPAD);
	writeByte(th);
	writeByte(tl);
	writeByte(config);
}

//
// You need to use this function to start a search again from the beginning.
// You do not need to do it for the first search, though you could.
//
void OneWire::reset_search()
{
  // reset the search state
  LastDiscrepancy = 0;
  LastDeviceFlag = false;
  LastFamilyDiscrepancy = 0;
  for(int i = 7; ; i--) {
    ROM_NO[i] = 0;
    if ( i == 0) break;
  }
}

// Setup the search to find the device type 'family_code' on the next call
// to search(*newAddr) if it is present.
//
void OneWire::target_search(uint8_t family_code)
{
   // set the search state to find SearchFamily type devices
   ROM_NO[0] = family_code;
   for (uint8_t i = 1; i < 8; i++)
      ROM_NO[i] = 0;
   LastDiscrepancy = 64;
   LastFamilyDiscrepancy = 0;
   LastDeviceFlag = false;
}

//
// Perform a search. If this function returns a '1' then it has
// enumerated the next device and you may retrieve the ROM from the
// OneWire::address variable. If there are no devices, no further
// devices, or something horrible happens in the middle of the
// enumeration then a 0 is returned.  If a new device is found then
// its address is copied to newAddr.  Use OneWire::reset_search() to
// start over.
//
// --- Replaced by the one from the Dallas Semiconductor web site ---
//--------------------------------------------------------------------------
// Perform the 1-Wire Search Algorithm on the 1-Wire bus using the existing
// search state.
// Return TRUE  : device found, ROM number in ROM_NO buffer
//        FALSE : device not found, end of search
//
uint8_t OneWire::search(uint8_t *newAddr, bool search_mode /* = true */)
{
   uint8_t id_bit_number;
   uint8_t last_zero, rom_byte_number, search_result;
   uint8_t id_bit, cmp_id_bit;

   unsigned char rom_byte_mask, search_direction;

   // initialize for search
   id_bit_number = 1;
   last_zero = 0;
   rom_byte_number = 0;
   rom_byte_mask = 1;
   search_result = 0;

   // if the last call was not the last one
   if (!LastDeviceFlag)
   {
      // 1-Wire reset
      if (!reset())
      {
         // reset the search
         LastDiscrepancy = 0;
         LastDeviceFlag = false;
         LastFamilyDiscrepancy = 0;
         return false;
      }

      // issue the search command
      if (search_mode == true) {
    	  writeByte(0xF0);   // NORMAL SEARCH
      } else {
    	  writeByte(0xEC);   // CONDITIONAL SEARCH
      }

      // loop to do the search
      do
      {
         // read a bit and its complement
         id_bit = readBit();
         cmp_id_bit = readBit();

         // check for no devices on 1-wire
         if ((id_bit == 1) && (cmp_id_bit == 1))
            break;
         else
         {
            // all devices coupled have 0 or 1
            if (id_bit != cmp_id_bit)
               search_direction = id_bit;  // bit write value for search
            else
            {
               // if this discrepancy if before the Last Discrepancy
               // on a previous next then pick the same as last time
               if (id_bit_number < LastDiscrepancy)
                  search_direction = ((ROM_NO[rom_byte_number] & rom_byte_mask) > 0);
               else
                  // if equal to last pick 1, if not then pick 0
                  search_direction = (id_bit_number == LastDiscrepancy);

               // if 0 was picked then record its position in LastZero
               if (search_direction == 0)
               {
                  last_zero = id_bit_number;

                  // check for Last discrepancy in family
                  if (last_zero < 9)
                     LastFamilyDiscrepancy = last_zero;
               }
            }

            // set or clear the bit in the ROM byte rom_byte_number
            // with mask rom_byte_mask
            if (search_direction == 1)
              ROM_NO[rom_byte_number] |= rom_byte_mask;
            else
              ROM_NO[rom_byte_number] &= ~rom_byte_mask;

            // serial number search direction write bit
            writeBit(search_direction);

            // increment the byte counter id_bit_number
            // and shift the mask rom_byte_mask
            id_bit_number++;
            rom_byte_mask <<= 1;

            // if the mask is 0 then go to new SerialNum byte rom_byte_number and reset mask
            if (rom_byte_mask == 0)
            {
                rom_byte_number++;
                rom_byte_mask = 1;
            }
         }
      }
      while(rom_byte_number < 8);  // loop until through all ROM bytes 0-7

      // if the search was successful then
      if (!(id_bit_number < 65))
      {
         // search successful so set LastDiscrepancy,LastDeviceFlag,search_result
         LastDiscrepancy = last_zero;

         // check for last device
         if (LastDiscrepancy == 0)
            LastDeviceFlag = true;

         search_result = true;
      }
   }

   // if no device found then reset counters so next 'search' will be like a first
   if (!search_result || !ROM_NO[0])
   {
      LastDiscrepancy = 0;
      LastDeviceFlag = false;
      LastFamilyDiscrepancy = 0;
      search_result = false;
   } else {
      for (int i = 0; i < 8; i++) newAddr[i] = ROM_NO[i];
   }
   return search_result;
  }
