#ifndef SENSORHOUSEKEEPING_H
#define SENSORHOUSEKEEPING_H

#include "rodos.h"
#include "topics.h"
#include "comminterfaces.h"

class SensorHousekeeping : public Thread
{
    hkData hk;
public:
    SensorHousekeeping();
    void configADC();
    void getTankPressure(uint16_t *presTank);
    void getValvesPressure(uint16_t *presValves);
    void getTemperatureData();
    void run();
};

#endif // SENSORHOUSEKEEPING_H
