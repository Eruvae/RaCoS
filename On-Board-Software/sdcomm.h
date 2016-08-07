#ifndef SDCOMM_H
#define SDCOMM_H

#include "rodos.h"
#include "comminterfaces.h"
#include "spihelper.h"

class SDComm
{
public:
    SDComm();
    int sendCommand(uint8_t command, uint32_t argument, uint8_t crc);
    void init();
    int read_sector_segment(uint32_t sector, uint8_t *buf);
    int write_sector_segment(uint32_t sector, uint8_t *buf);

};

#endif // SDCOMM_H
