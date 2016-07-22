#pragma once

#include "rodos.h"

extern HAL_SPI imu1;
extern HAL_SPI imu2;

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
	int initIMUs();
	int configIMUs();
	int getIMU1();
	int getIMU2();
	void fusionFilter();
	void sendData();
	void init();
	void run();
	void sendStatus();
	
};