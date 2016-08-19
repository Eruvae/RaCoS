#ifndef TELEMETRY_H
#define TELEMETRY_H

#include "rodos.h"
#include "topics.h"
#include "structs.h"
#include "comminterfaces.h"
#include "murmur.h"

class Telemetry : public Thread
{
	private:
		IMUdata imu;
		HkData hk;
		CmdData calc;
		Mode mode;
		bool testMode;
		
    public:
		void run();
		int encodeIMU(char *buffer);
		int encodePresTemp(char *buffer);
		int encodeCalc(char *buffer);
		uint32_t generateChecksum(char *buffer, int size);
};

extern Telemetry telemetry;

#endif
