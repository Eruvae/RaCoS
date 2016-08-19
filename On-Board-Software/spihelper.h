#ifndef SPIHELPER_H
#define SPIHELPER_H

#include "rodos.h"

enum SPI_SS {IMU1, IMU2, SD};
//extern Semaphore spi_comm_running;

/*HAL_GPIO ss_imu1;
HAL_GPIO ss_imu2;
HAL_GPIO ss_sd;*/

class SPIHelper : public Initiator
{
    bool slaveSelected;
public:
    SPIHelper();
    int selectSlave(SPI_SS select);
    void disableSlaves();

    void init();
};

extern SPIHelper spiHelper;

#endif // SPIHELPER_H
