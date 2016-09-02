#ifndef SPIHELPER_H
#define SPIHELPER_H

#include "rodos.h"

enum SPI_SS {NO_SLAVE, IMU1, IMU2, SD};
//extern Semaphore spi_comm_running;
extern const uint8_t xff[512];

/*HAL_GPIO ss_imu1;
HAL_GPIO ss_imu2;
HAL_GPIO ss_sd;*/

class SPIHelper : public Initiator
{
    SPI_SS selectedSlave;
public:
    SPIHelper();
    bool isSlaveSelected(SPI_SS slave);
    int selectSlave(SPI_SS select);
    void disableSlaves();

    void init();
};

extern SPIHelper spiHelper;

#endif // SPIHELPER_H
