#include "healthwatchdog.h"
#include <cstdlib>
#include <new>

HealthWatchdog healthWatchdog;

HealthWatchdog::HealthWatchdog()
{
	pending = false;
	badCount = 0;
	noCriticalErrorOccurred = true;
	emergencyCutoff = false;
	sensorIMUStatus = NO_INIT;
	housekeepingStatus = NO_INIT;
	controlLoopStatus = NO_INIT;
	storageControllerStatus = NO_INIT;
	telemetryStatus = NO_INIT;
}

void HealthWatchdog::run()
{
	//modulStateTopic.publishConst(0);
    setPeriodicBeat(0, 500*MILLISECONDS);
    while(1)
    {
    	uint16_t status = encodeModulStates();
    	modulStateTopic.publish(status);
        suspendUntilNextBeat();
    }
}

bool HealthWatchdog::emergencyCutoffCheck()
{
    return emergencyCutoff;
}

void HealthWatchdog::sendCutoff(bool state)
{
    CmdData out;
    out.valveState = state? AH_EmergencyOpen:AH_EmergencyClose;
    out.vot1 = 0;
    out.vot2 = 0;
    controlTopic.publish(out);
    emergencyCutoff = state;
}

uint16_t HealthWatchdog::encodeModulStates()
{
	uint16_t states =	(sensorIMUStatus & 0b111) |
						((housekeepingStatus & 0b111) << 3) |
						((controlLoopStatus & 0b111) << 6) |
						((storageControllerStatus & 0b111) << 9) |
						((telemetryStatus & 0b111) << 12);
	return states;
}

void HealthWatchdog::setIMUStatus(Status error)
{
	sensorIMUStatus = error;
}

void HealthWatchdog::setHousekeepingStatus(Status error)
{
	housekeepingStatus = error;
}

void HealthWatchdog::setControlLoopStatus(Status error)
{
	controlLoopStatus = error;
}

void HealthWatchdog::setStorageControllerStatus(Status error)
{
	storageControllerStatus = error;
}

void HealthWatchdog::setTelemetryStatus(Status error)
{
	telemetryStatus = error;
}
