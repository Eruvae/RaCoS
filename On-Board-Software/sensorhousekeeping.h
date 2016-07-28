#ifndef SENSORHOUSEKEEPING_H
#define SENSORHOUSEKEEPING_H

#include "rodos.h"
#include "comminterfaces.h"

class SensorHousekeeping : public Thread
{
public:
    SensorHousekeeping();
    void getPressureData();
    void getTemperatureData();
    void run();
};

#endif // SENSORHOUSEKEEPING_H
