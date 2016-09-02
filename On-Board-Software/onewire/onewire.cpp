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

	if (result >= 0x10 && result <= 0x90)
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
    uint8_t crc = 0;

    while (len--)
        crc = dscrc_table[crc ^ *addr++];

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
