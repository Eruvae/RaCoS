#include "spihelper.h"

SPIHelper spiHelper;

HAL_GPIO ss_imu1(GPIO_015); //TODO
HAL_GPIO ss_imu2(GPIO_016); //TODO
HAL_GPIO ss_sd(GPIO_017); //TODO

SPIHelper::SPIHelper()
{

}

void SPIHelper::init()
{
    ss_imu1.init(true, 1, 1);
    ss_imu2.init(true, 1, 1);
    ss_sd.init(true, 1, 1);

    slaveSelected = false;
}

int SPIHelper::selectSlave(SPI_SS select)
{
    if (slaveSelected)
        return -1;

    slaveSelected = true;

    switch(select)
    {
    case IMU1:
        ss_imu2.setPins(true);
        ss_sd.setPins(true);
        ss_imu1.setPins(false);
    case IMU2:
        ss_imu1.setPins(true);
        ss_sd.setPins(true);
        ss_imu2.setPins(false);
    case SD:
        ss_imu1.setPins(true);
        ss_imu2.setPins(true);
        ss_sd.setPins(false);
    }

    return 0;
}

void SPIHelper::disableSlaves()
{
    ss_imu1.setPins(true);
    ss_imu2.setPins(true);
    ss_sd.setPins(true);

    slaveSelected = false;
}
