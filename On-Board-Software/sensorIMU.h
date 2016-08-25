#ifndef SENSORIMU_H
#define SENSORIMU_H

#include "rodos.h"

#include "comminterfaces.h"
#include "spihelper.h"
#include "topics.h"
#include "structs.h"

struct __attribute__((packed)) IMUReadStruct
{
	int16_t accData[3];
	int16_t tempData;
	int16_t gyroData[3];
};

class SensorIMU : public Thread
{
private:
	double GxHistory1[5];
	double GxHistory2[5];
	double GyHistory1[5];
	double GyHistory2[5];
	double GzHistory1[5];
	double GzHistory2[5];
	
	IMUdata imu;

public:
    //int initIMUs();
	int configReg(SPI_SS id, uint8_t reg, uint8_t config);
    int initIMU(SPI_SS id);
    int getIMU(SPI_SS id, IMUReadStruct *buffer);
    int resetIMU(SPI_SS id);
    //void fusionFilter();
    //void sendData();
    //void init();
	void run();
    //void sendStatus();
	
};

//extern SensorIMU sensorIMU;

#endif
