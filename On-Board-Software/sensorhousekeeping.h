#ifndef SENSORHOUSEKEEPING_H
#define SENSORHOUSEKEEPING_H

#include "rodos.h"
#include "topics.h"
#include "comminterfaces.h"

class SensorHousekeeping : public Thread
{
    hkData hk;
    uint8_t crc8(const void *vptr, int len);
public:
    SensorHousekeeping();
    void configADC();
    void getTankPressure(uint16_t *presTank);
    void getValvesPressure(uint16_t *presValves);
    void configTempSensor(uint64_t rom_code);
    void initTemperatureConv();
    int16_t getTemperatureData(uint64_t rom_code);
    float convertTemperature(int16_t temp);
    void run();
};

extern SensorHousekeeping sensorHousekeeping;

#endif // SENSORHOUSEKEEPING_H
