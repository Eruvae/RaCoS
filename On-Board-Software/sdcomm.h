#ifndef SDCOMM_H
#define SDCOMM_H

#include "rodos.h"
#include "comminterfaces.h"
#include "spihelper.h"

class SDComm
{
	bool is_initialized;
public:
    SDComm();
    int sendCommand(uint8_t command, uint32_t argument, uint8_t crc = 0xFF);
    int init();
    int read_sector_segment(uint32_t sector, uint8_t *buf);
    int write_sector_segment(uint32_t sector, const uint8_t *buf);
    int get_status();

};

extern SDComm sdcomm;

#endif // SDCOMM_H
