#include "healthwatchdog.h"
#include <cstdlib>
#include <new>

HealthWatchdog healthWatchdog;

/*SensorHousekeeping *sensorHousekeeping;
SensorIMU *sensorIMU;
Telecommand *telecommand;
Telemetry *telemetry;
ControlLoop *controlLoop;
StorageController *storageController;*/

HealthWatchdog::HealthWatchdog()
{
	/*sensorHousekeeping = (SensorHousekeeping*)malloc(sizeof(SensorHousekeeping));
	new (sensorHousekeeping) SensorHousekeeping();
	sensorIMU = (SensorIMU*)malloc(sizeof(SensorIMU));
	new (sensorIMU) SensorIMU();
	telemetry = (Telemetry*)malloc(sizeof(Telemetry));
	new (telemetry) Telemetry();*/
	housekeepingPending = false;
	sensorIMUpending = false;
	actuatorHandlerpending = false;
	controlLoopPending = false;
	pending = false;
	badCount = 0;
	noCriticalErrorOccurred = false;
	emergencyCutoff = false;
}

void HealthWatchdog::run()
{
	modulStateTopic.publishConst(0);
    setPeriodicBeat(0, 500*MILLISECONDS);
    while(1)
    {
        badCount += housekeepingPending? 1:0;
        badCount += sensorIMUpending? 1:0;
        badCount += actuatorHandlerpending? 1:0;
        badCount += controlLoopPending? 1:0;
        if(badCount>HWTD_ErrorThreshold){
            Status stat;
            stat.errorcode = Timeout;
            stat.id = HWTD_ID;
            stat.systemBad = true;
            healthTopic.publish(stat);
        }
        suspendUntilNextBeat();
    }
}

void HealthWatchdog::initThreads()
{
}

bool HealthWatchdog::selfcheck()
{
    bool t1 = housekeepingPending;
    bool t2 = sensorIMUpending;
    bool t3 = actuatorHandlerpending;
    bool t4 = controlLoopPending;
    bool success = false;

    housekeepingPending = true;
    sensorIMUpending = true;
    actuatorHandlerpending = true;
    controlLoopPending = true;

    if(housekeepingPending && sensorIMUpending && actuatorHandlerpending && controlLoopPending)success = true;

    housekeepingPending = false;
    sensorIMUpending = false;
    actuatorHandlerpending = false;
    controlLoopPending = false;

    if(!housekeepingPending && !sensorIMUpending && !actuatorHandlerpending && !controlLoopPending)success = true;
    else success = false;

    housekeepingPending = t1;
    sensorIMUpending = t2;
    actuatorHandlerpending = t3;
    controlLoopPending = t4;
    return success;
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
