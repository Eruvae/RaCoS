#pragma once

#include "rodos.h"
#include "topics.h"
#include "structs.h"

class Telemetry : public Thread
{
	private:
		IMUdata imu;
		hkData hk;
		cmdData calc;
		uint8_t mode;
		
	public:
		void init();
		void run();
		int encodeIMU(char *buffer);
		int encodePresTemp(char *buffer);
		int encodeCalc(char *buffer);
		int generateChecksum(char *buffer, int size);
};