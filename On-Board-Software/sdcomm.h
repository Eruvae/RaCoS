#ifndef SDCOMM_H
#define SDCOMM_H

#include "comminterfaces.h"
#include "spihelper.h"

class SDComm
{
public:
    SDComm();
    int sendCommand(uint8_t command, uint32_t argument);
    void init();
    void write();
    void read();

};

#endif // SDCOMM_H
