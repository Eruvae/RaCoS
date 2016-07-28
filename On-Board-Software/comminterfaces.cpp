#include "comminterfaces.h"

HAL_SPI spi_bus(SPI_IDX2);
HAL_I2C i2c_bus(I2C_IDX1);
HAL_UART teleUART(UART_IDX1);

CommInterfaces commInit;

CommInterfaces::CommInterfaces()
{
}

void CommInterfaces::init()
{
    spi_bus.init(); // TODO (optional): Buadrade angeben, Mode per config; TODO: error handling
    i2c_bus.init();
    teleUART.init();
}
