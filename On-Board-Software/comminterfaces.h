#ifndef COMMINTERFACES_H
#define COMMINTERFACES_H

#include "rodos.h"

extern HAL_SPI spi_bus;
extern HAL_I2C i2c_bus;
extern HAL_UART teleUART;

class CommInterfaces : public Initiator
{
public:
    CommInterfaces();
    void init();
};

// extern CommInterfaces commInit;

#endif // COMMINTERFACES_H
