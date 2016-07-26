#include "spihelper.h"

SPIHelper spiHelper;

HAL_SPI spi_bus(SPI_IDX2);
HAL_GPIO ss_imu1(GPIO_015); //TODO
HAL_GPIO ss_imu2(GPIO_016); //TODO
HAL_GPIO ss_sd(GPIO_017); //TODO

SPIHelper::SPIHelper()
{

}

void SPIHelper::init()
{
    spi_bus.init(); // TODO (optional): Buadrade angeben, Mode per config; TODO: error handling
    ss_imu1.init(true, 1, 0);
    ss_imu2.init(true, 1, 0);
    ss_sd.init(true, 1, 0);
}

void SPIHelper::selectSlave(SPI_SS select)
{
    switch(select)
    {
    case IMU1:
        ss_imu2.setPins(false);
        ss_sd.setPins(false);
        ss_imu1.setPins(true);
    case IMU2:
        ss_imu1.setPins(false);
        ss_sd.setPins(false);
        ss_imu2.setPins(true);
    case SD:
        ss_imu1.setPins(false);
        ss_imu2.setPins(false);
        ss_sd.setPins(true);
    }
}

void SPIHelper::disableSlaves()
{
    ss_imu1.setPins(false);
    ss_imu2.setPins(false);
    ss_sd.setPins(false);
}
