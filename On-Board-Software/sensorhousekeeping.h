#ifndef SENSORHOUSEKEEPING_H
#define SENSORHOUSEKEEPING_H

#include "rodos.h"
#include "topics.h"
#include "comminterfaces.h"

class SensorHousekeeping : public Thread
{
	HkData hk;
    uint8_t crc8(const void *vptr, int len);
public:
    SensorHousekeeping();
    bool configADC();
    bool getTankPressure(uint16_t *presTank);
    bool getValvesPressure(uint16_t *presValves);
    void configTempSensor(const uint8_t *rom_code);
    void initTemperatureConv();
    int16_t getTemperatureData(const uint8_t *rom_code);
    void reset_i2c();
    void run();
};

//extern SensorHousekeeping sensorHousekeeping;

#endif // SENSORHOUSEKEEPING_H
