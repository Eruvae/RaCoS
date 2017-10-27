#ifndef TELEMETRY_H
#define TELEMETRY_H

#include "rodos.h"
#include "topics.h"
#include "structs.h"
#include "comminterfaces.h"
#include "murmur.h"

//#define ACTIVATE_DEBUG_DL

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
		void dl_debug(const char *x)
		{
			#ifndef ACTIVATE_DEBUG_DL
				return;
			#endif
			DpDebug dbg;
			dbg.sync = DL_DEBUG_ID;
			strcpy(dbg.text, (x));
			dbg.check = Murmur::mm_hash_32((uint8_t*)&dbg, sizeof(DpDebug) - 4);
			dlDebugTopic.publish(dbg);
		}

};

extern Telemetry telemetry;

#endif
