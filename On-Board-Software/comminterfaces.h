#ifndef COMMINTERFACES_H
#define COMMINTERFACES_H

#include "rodos.h"
#include "onewire/onewire.h"

#define SPI_BAUDRATE 1000000

extern HAL_SPI spi_bus;
extern HAL_I2C i2c_bus;
extern HAL_UART teleUART;
extern HAL_UART onewire_uart;
extern OneWire oneWire;

class CommInterfaces : public Initiator
{
public:
    CommInterfaces();
    void init();
};

extern CommInterfaces commInit;

#endif // COMMINTERFACES_H
