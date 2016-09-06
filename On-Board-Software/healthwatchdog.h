#ifndef HEALTHWATCHDOG_H
#define HEALTHWATCHDOG_H

#define HWTD_ErrorThreshold 5
#define Timeout 25
#define HWTD_ID 5
#define AH_EmergencyOpen 1234
#define AH_EmergencyClose 4567

#include "rodos.h"
#include "topics.h"
#include "structs.h"

class HealthWatchdog: public Thread {
private:
	bool pending;
	uint32_t badCount;
	bool noCriticalErrorOccurred;
	bool emergencyCutoff;
	int sensorIMUStatus;
	int actuatorHandlerStatus;
	int controlLoopStatus;
public:
	HealthWatchdog();
	void run();
	void initThreads();
	bool selfcheck();
	bool emergencyCutoffCheck();
	void sendCutoff(bool state);
	void setIMUStatus(int error);
	void setActuatorHandlerStatus(int error);
	void setControlLoopStatus(int error);
};

extern HealthWatchdog healthWatchdog;

#endif // HEALTHWATCHDOG_H
