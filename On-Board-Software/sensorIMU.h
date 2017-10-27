#ifndef SENSORIMU_H
#define SENSORIMU_H

#include "rodos.h"
#include "stdlib.h"

#include "comminterfaces.h"
#include "spihelper.h"
#include "topics.h"
#include "structs.h"
#include "math.h"

#define GYRO_FACTOR250         0.00762939453125    // Max. Scale: 250 deg/s
#define GYRO_FACTOR500		0.01525878906
#define GYRO_FACTOR1000		0.03051757812
#define GYRO_FACTOR2000		0.06103515625
#define ACC_FACTOR          0.00048828125       // Max. Scale: 16g
//#define ACC_FACTOR          0.00006103515625
#define MAG_FACTOR			0.15

struct __attribute__((packed)) IMUReadStruct
{
	int16_t accData[3];
	int16_t tempData;
	int16_t gyroData[3];
};

class SensorIMU : public Thread
{
private:
	volatile bool calibrateIMU;
	double GxHistory1[5];
	double GxHistory2[5];
	double GyHistory1[5];
	double GyHistory2[5];
	double GzHistory1[5];
	double GzHistory2[5];
	double filterXLast[5];
	double filterYLast[5];
	double filterZLast[5];
	bool IMUworking[2];
    uint64_t retryCycle;
    double calibG1X[4]; //0 = 250 dps; 1 = 500 dps; 2 = 1000 dps; 3 = 2000 dps;
    double calibG1Y[4];
    double calibG1Z[4];
    double calibG2X[4];
    double calibG2Y[4];
    double calibG2Z[4];
    int fusionCycle;
	
	double scale;
	uint8_t scale_cfg;
	uint8_t scale_select;

	IMUdata imu;

public:
    //int initIMUs();
	void setIMUCalibration();
	SensorIMU(const char *name, const long priority) : Thread(name, priority),
				calibrateIMU(false), scale(GYRO_FACTOR2000), scale_cfg(0), retryCycle(0){};
	int readReg(SPI_SS id, uint8_t reg, void *buf, uint32_t cnt);
	int configReg(SPI_SS id, uint8_t reg, uint8_t config);
    int initIMU(SPI_SS id);
    int getIMU(SPI_SS id, IMUReadStruct *buf);
    //int getMagnetometer(SPI_SS id, void *buffer);
    int resetIMU(SPI_SS id);
    int calibrate();
    void fusionFilter(IMUdata &imu);
    void autoscale();
    //void thermalCalibration();
    double getScale();
    //void sendData();
    //void init();
	void run();
    //void sendStatus();
	
};

extern SensorIMU sensorIMU;

#endif
