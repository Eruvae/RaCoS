#include "spihelper.h"

SPIHelper spiHelper;

HAL_GPIO ss_imu1(GPIO_038); // PC06
HAL_GPIO ss_imu2(GPIO_039); // PC07
HAL_GPIO ss_sd(GPIO_028); // PB12

Semaphore spi_comm_running;

SPIHelper::SPIHelper()
{
	slaveSelected = false;
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
        break;
    case IMU2:
        ss_imu1.setPins(true);
        ss_sd.setPins(true);
        ss_imu2.setPins(false);
        break;
    case SD:
        ss_imu1.setPins(true);
        ss_imu2.setPins(true);
        ss_sd.setPins(false);
        break;
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
